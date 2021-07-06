#include <iostream>
#include <libusb-1.0/libusb.h>
using namespace std;


//prototype functions
int choose_device(ssize_t cnt, libusb_device **devs);
void print_name_id(libusb_device *dev, ssize_t num);
void dataProc(libusb_transfer *transfer);
typedef void(*libusb_transfer_cb_fn) (struct libusb_transfer *tranfer);
libusb_transfer_cb_fn callback = &dataProc;
int choose_interface(libusb_device *dev);
int choose_alternate_setting(libusb_device *dev, int interface);
int endpoint_selector(libusb_device *dev, int interface, int alternate_set);


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

	libusb_get_config_descriptor(device, 0, &config);
	
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;


	int interface_num =  choose_interface(device);
	int alt_set_num = choose_alternate_setting(device, interface_num);
	int endpt_num = endpoint_selector(device, interface_num, alt_set_num);


	inter = &config->interface[interface_num];
	interdesc = &inter->altsetting[alt_set_num];
	epdesc = &interdesc->endpoint[endpt_num];

	libusb_free_device_list(devs, device_num);

    libusb_device_handle *handle;
	libusb_open(device, &handle);

    libusb_transfer *mainTransfer = libusb_alloc_transfer(0);

    unsigned char data[4096];

	unsigned char endpoint = epdesc->bEndpointAddress;

	cout << "Please Enter Your Timeout Durration (ms): " << endl;
	string input;
	getline(cin, input);
	unsigned int timeout = stoi(input);

	libusb_fill_bulk_transfer(mainTransfer,handle,endpoint,data,(int)sizeof(data),callback,NULL, timeout);

	int transferReturn = libusb_submit_transfer(mainTransfer);

	if(transferReturn == 0){
		cout << "Transfer Successful with code 0" << endl;
	}else{
		cout << "Transfer Error, Code: " << transferReturn << " " << libusb_error_name(transferReturn) << ": " << libusb_strerror(transferReturn) << endl;
	}

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

int choose_interface(libusb_device *dev){
	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);

	cout <<"Interfaces: " << endl;

	const libusb_interface *inter;

	for(int i  = 0; i < (int)config->bNumInterfaces; i++){
		inter = &config->interface[i];
 		cout<<"Interface "<< i << ": " <<"Number of Alternate Settings: "<<inter->num_altsetting<<endl;
	}

	cout << "Please select an Interface using its number" << endl;

	string input;
	getline(cin, input);
	int returnVal = stoi(input);

	return returnVal;
}


int choose_alternate_setting(libusb_device *dev, int interface){

	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);

	cout << config->bConfigurationValue << endl;

	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;

	inter = &config->interface[interface];

	for(int i = 0; i < inter->num_altsetting; i++){
		interdesc = &inter->altsetting[i];
		cout << "Alternate Setting " << i << ": ";
		cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
		cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<< endl;
	}

	cout << "Please Choose an Alternate Setting Number: " << endl;

	string input;
	getline(cin, input);
	int returnVal = stoi(input);

	return returnVal;
}

int endpoint_selector(libusb_device *dev, int interface, int alternate_set){
	libusb_config_descriptor *config;
 	libusb_get_config_descriptor(dev, 0, &config);

	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;

	inter = &config->interface[interface];
	interdesc = &inter->altsetting[alternate_set];

	cout << "Endpoints: " << endl;

	for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
		epdesc = &interdesc->endpoint[k];
		cout << "Endpoint " << k << ": ";
		cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
		cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<endl;
	}

	cout << "Please Select an Endpoint Using its Number:" << endl;
	
	string input;
	getline(cin, input);

	int returnVal = stoi(input);

	return returnVal;
}

void dataProc(libusb_transfer *transfer){
	unsigned char *buffer = transfer->buffer;
	if (buffer[0] != NULL){
		cout << buffer << endl;
	}else{
		cout << "Error in buffer collection/output" << endl;
	}
}
