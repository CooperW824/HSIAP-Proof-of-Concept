#
import re
import subprocess

device_re = re.compile(b"Bus\s+(?P<bus>\d+)\s+Device\s+(?P<device>\d+).+ID\s(?P<id>\w+:\w+)\s(?P<tag>.+)$", re.I)
df = subprocess.check_output("lsusb")
devices = []
for i in df.split(b'\n'):
  if i:
    info = device_re.match(i)
    if info:
      dinfo = info.groupdict()
      dinfo['device'] = '/dev/bus/usb/%s/%s' % (dinfo.pop('bus'), dinfo.pop('device'))
      devices.append(dinfo)

print(devices)
print("Please select the number of a usb device:")
count = 0
for i in devices:
  print(str(count)+": "+ str(i["tag"])[2:len(str(i["tag"]))-1] + ": " + re.sub("[b']", "", i["device"]))
  count+=1


def get_usb_info(number: int, devices_list: list):
    device = devices_list[number]
    device_ids = str(device["id"])
    device_ids = device_ids[2:11]
    return device_ids.split(":")


def hex_str_to_hex(hex_str: str):
    hex_int = int(hex_str, 16)
    return hex(hex_int)


needed_Ids = get_usb_info(int(input("Please select the Number of the device: \n")), devices)

import os

os.environ["PYUSB_DEBUG"] = "debug"

import usb.backend.libusb1
vendorId = hex_str_to_hex(needed_Ids[0])
productId = hex_str_to_hex(needed_Ids[1])

dev = usb.core.find(idVendor=0x46d, idProduct=0xc534)


if dev is None:
    raise ValueError("No Device Found")
else:
    ep = dev[0].interfaces()[0].endpoints()[0]
    i = dev[0].interfaces()[0].bInterfaceNumber

    dev.reset()
    reattach = False
    if dev.is_kernel_driver_active(i):
        print("Kernel Disabled")
        dev.detach_kernel_driver(i)
        reattach = True
        usb.util.dispose_resources(dev)

    dev.reset()

    try:
        dev.reset()
        dev.set_configuration()
    except usb.core.USBError:
        print("resource busy")

    try:
        eaddr = ep.bEndpointAddress
        r = dev.read(eaddr, 1024)
        print(len(r))
    except usb.core.USBTimeoutError:
        print("USB Timeout")

    print(reattach)
    dev.reset()
    if reattach:
        # dev.reset()
        dev.attach_kernel_driver(i)

print(r)
