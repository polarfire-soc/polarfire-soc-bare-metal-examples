import can
import time

def send_can_message(channel, can_id, data):
    """Send a single CAN message."""
    #print("Sending data:", ' '.join([hex(byte) for byte in data]))  # Print the data being sent
    bus = can.interface.Bus(channel=channel, bustype='socketcan')
    message = can.Message(arbitration_id=can_id, data=data, is_extended_id=False)
    bus.send(message)

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

def send_data_from_wic_file(file_path, channel, can_id, small_delay, large_delay, large_delay_threshold=512):
    """Send data from a binary file over CAN, introducing delays after certain thresholds."""
    total_bytes_sent = 0
    for data in read_data_from_file(file_path):
        send_can_message(channel, can_id, data)
        time.sleep(small_delay)
        total_bytes_sent += len(data)
        if total_bytes_sent >= large_delay_threshold:
            time.sleep(large_delay)
            total_bytes_sent = 0  # Reset the counter

# Usage example
wic_file_path = 'mpfs-rpmsg-master.bin'  # Replace with the path to your payload file
can_channel = 'can0'
fixed_can_id = 0x00
small_delay_seconds = 0.0002
large_delay = 0.02
send_data_from_wic_file(wic_file_path, can_channel, fixed_can_id, small_delay_seconds, large_delay)
