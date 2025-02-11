import can
import time
import sys

can_bus = None
start_time = time.time()  # Get the start time
last_message_time = 0


def init_can(channel):
    global can_bus
    try:
        can_bus = can.interface.Bus(channel=channel, bustype='socketcan')
    except Exception as e:
        can.log(f"Error to connect to the CAN bus: {str(e)}")


def send_can_message(can_id, data, control_flow):
    global last_message_time
    last_message_time = time.time()
    if len(data) == 1:
        data = data + [0x00, 0x00, 0x03, 0x04, 0x05, 0x06, 0x07]
    elif len(data) == 2:
        data = data + bytes([0x00, 0x03, 0x04, 0x05, 0x06, 0x07])
    elif len(data) == 3:
        data = data + bytes([0x03, 0x04, 0x05, 0x06, 0x07])

    message = can.Message(arbitration_id=can_id, data=data, is_extended_id=False)
    can_bus.send(message)
    time.sleep(delay_seconds)
    if control_flow:
        if not receive_messages(data[0] + 0x040, timeout=2):  # Waits 2 sec to receive the message
            print("Error: No control flow message 0x%x received within 2 seconds. Script terminated." % (data[0] + 0x40))
            sys.exit()  # The script ends


def read_data_from_file(file_path, chunk_size=8):
    """Read and reorder data from a file in specified chunk sizes."""
    with open(file_path, 'rb') as file:
        while True:
            chunk = file.read(chunk_size)
            if not chunk:
                break
            reordered_chunk = [chunk[i:i + 2][::-1] for i in range(0, len(chunk), 2)]
            reordered_chunk = [reordered_chunk[i + 1] + reordered_chunk[i] for i in range(0, len(reordered_chunk), 2)]
            yield b''.join(reordered_chunk)

    data, file_size = read_data_from_file(file_path)
    print(f"Size of the file: {file_size} bytes")
    total_bytes_sent = 0

    # Send the total size of the file
    total_chunks = file_size // 512
    remaining_bytes = file_size % 512
    if remaining_bytes > 0:
        total_chunks += 1

    print(f"The total blocks to write are: {total_chunks}")
    total_chunks_msg = total_chunks.to_bytes(2, byteorder='big')
    message = bytes([0x33]) + total_chunks_msg
    send_can_message(fixed_can_id, message, control_flow=True)


def send_data_from_wic_file(file_path,  small_delay, large_delay_threshold=512):
    """Send data from a binary file over CAN, introducing delays after certain thresholds."""
    total_bytes_sent = 0
    send_can_message(fixed_can_id, [0x44], control_flow=True)  # SOB
    for data in read_data_from_file(file_path):
        send_can_message(fixed_can_id, data,  control_flow=False)
        time.sleep(small_delay)
        total_bytes_sent += len(data)
        if total_bytes_sent >= large_delay_threshold:
            send_can_message(fixed_can_id, [0x55], control_flow=True)  # EOB
           # time.sleep(0.02)
            total_bytes_sent = 0  # Reset the counter
            send_can_message(fixed_can_id, [0x44], control_flow=True)  # SOB
    print("All data has been sent successfully")

def receive_messages(data_expected, timeout):
    """Receive a single CAN message within a specified timeout."""
    global start_time
    global last_message_time  # Access the time of the last message
    timestamp = str((time.time() - start_time))[:5]
    while True:
        received_message = can_bus.recv(timeout = timeout)  # Wait for a message with the specified timeout
        if received_message is not None:
            if ((received_message.arbitration_id == 0x4) or (received_message.arbitration_id == 0x04) or (received_message.arbitration_id == 0x040)):
                    if data_expected == received_message.data[0]:
                        if (0x03 == received_message.data[3] and
                        0x04 == received_message.data[4] and
                        0x05 == received_message.data[5] and
                        0x06 == received_message.data[6] and
                        0x07 == received_message.data[7]):
                            # print(f"Rx ID: {received_message.arbitration_id}, Time: {timestamp}, Data:",
                            # ' '.join([hex(byte) for byte in received_message.data]))
                            return received_message  # Return the received message if available
                        else:
                            print(" wrong data received")
                    else:
                        print(" wrong ID data_expected")

            elif (received_message.arbitration_id == 0x040 and
                    0x7F == received_message.data[0]):
                print(" Negative Response")
                return None
            else:
                print(f"received_message.arbitration_id:", {hex(received_message.arbitration_id)})
                return None
        elif time.time() - last_message_time >= timeout:
            return None  # Return None if timeout is reached


# Usage example
wic_file_path = 'mpfs-rpmsg-master.bin'  # Replace with the path to your payload file
can_channel = 'can0'
fixed_can_id = 0x000
delay_seconds = 0.001
init_can(can_channel)
send_can_message(fixed_can_id, [0x11], control_flow=True) #SOR
send_data_from_wic_file(wic_file_path, delay_seconds)
send_can_message(fixed_can_id, [0x22], control_flow=True) #EOR
