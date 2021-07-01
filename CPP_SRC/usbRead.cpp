#include <iostream>
#include <libusb-1.0/libusb.h>
using namespace std;


//prototype functions
int choose_device(ssize_t cnt, libusb_device **devs);
void print_name_id(libusb_device *dev, ssize_t num);

int main() {
	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	libusb_context *ctx = NULL; //a libusb session
	int r; //for return values
	
	r = libusb_init(&ctx); //initialize a library session
	libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, 4 );
	if(r < 0) {
		cout<<"Init Error "<<r<<endl; //there was an error
				return 1;
	}
	ssize_t cnt; //holding number of devices in list
	

	cnt = libusb_get_device_list(ctx, &devs); //get the list of devices

	if(cnt < 0) {
		cout<<"Get Device Error"<<endl; //there was an error
	}

	cout<<cnt<<" Devices in list."<<endl; //print total number of usb devices
	
	int device_num = choose_device(cnt, devs);
	libusb_device *device = devs[device_num];
	libusb_config_descriptor *config;

	libusb_free_device_list(devs,device_num);

    libusb_device_handle *handle;
	libusb_open(device, &handle);

    libusb_close(handle);
	libusb_exit(ctx); //close the session
	return 0;
}

int choose_device(ssize_t cnt, libusb_device **devs){

	libusb_device_descriptor desc;
	ssize_t i = 0;

	cout << "Please choose the number of a device to read from: " << endl;

	for(i = 0; i < cnt; i++){
		print_name_id(devs[i], i);
	}

	string input;
	getline(cin, input);

	int returnVal = stoi(input);

	return returnVal;
}

void print_name_id(libusb_device *dev, ssize_t num){

	libusb_device_descriptor desc;
	int r  =  libusb_get_device_descriptor(dev, &desc);

	if (r < 0){
		cout << "Error getting device info" << endl;
		return;
	}

	cout << num << ": " << hex <<desc.idVendor << ":" << hex <<(int)desc.idProduct << endl;

}