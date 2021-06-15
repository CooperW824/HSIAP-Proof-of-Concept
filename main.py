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


needed_Ids = get_usb_info(int(input("Please select the Number of the device: \n")), devices)

# import usb.core
# import usb.util
# # find USB devices
# dev = usb.core.find(find_all=True)
# # loop through devices, printing vendor and product ids in decimal and hex
# for cfg in dev:
#   print('Decimal VendorID=' + str(cfg.idVendor) + ' & ProductID=' + str(cfg.idProduct))
#   print('Hexadecimal VendorID=' + hex(cfg.idVendor) + ' & ProductID=' + hex(cfg.idProduct))
#   print(usb.util.find_descriptor(cfg))
