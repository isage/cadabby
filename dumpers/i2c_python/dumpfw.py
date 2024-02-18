import smbus
import time
bus = smbus.SMBus(1)
address = 0x55
rom_address = 0x0B

def enter_rom(bus):
    bus.write_word_data(address, 0x00, 0x0F00)
    time.sleep(1)

def exit_rom(bus):
    bus.write_byte_data(rom_address, 0x00, 0x0F)
    bus.write_byte_data(rom_address, 0x64, 0x0F)
    bus.write_byte_data(rom_address, 0x65, 0x00)
    time.sleep(1)

def read_row(bus, row):
    print("row: 0x{0:04x}".format(row))
    bus.write_byte_data(rom_address, 0x00, 0x00)

    bus.write_word_data(rom_address, 0x01, row) #row

    checksum = (row & 0xFF) + ((row >> 8) & 0xFF)
    bus.write_word_data(rom_address, 0x64, checksum) #checksum
    block = []
    #0x04 - 0x63 = 96 bytes
    for i in range(96):
        block.append(bus.read_byte_data(rom_address, 0x04+i))
    time.sleep(0.02)
    return block


status = bus.read_word_data(address, 0x00)
print("status: 0x{0:04x}".format(status))

enter_rom(bus)

#dump fw
fp = open("slim.fg.fw", "wb")

for i in range(512):
    block = read_row(bus, i)
    fp.write(bytearray(block))

fp.close()

exit_rom(bus)

status = bus.read_word_data(address, 0x00)
print("status 0x{0:04x}".format(status))
