#include <iostream>
#include <libusb-1.0/libusb.h>
using namespace std;


//prototype functions
int choose_device(ssize_t cnt, libusb_device **devs);
void print_name_id(libusb_device *dev, ssize_t num);
// int choose_interface(libusb_device *dev);
// int choose_alternate_setting(libusb_device *dev, int interface);
// int endpoint_selector(libusb_device *dev, int interface, int alternate_set);
//void printdev(libusb_device *dev); //prototype of the function

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

	// const libusb_interface *inter;
	// const libusb_interface_descriptor *interdesc;
	// const libusb_endpoint_descriptor *epdesc;


	// int interface_num =  choose_interface(device);
	// int alt_set_num = choose_alternate_setting(device, interface_num);
	// int endpt_num = endpoint_selector(device, interface_num, alt_set_num);


	// inter = &config->interface[interface_num];
	// interdesc = &inter->altsetting[alt_set_num];
	// epdesc = &interdesc->endpoint[endpt_num];

	// unsigned char endpt = epdesc->bEndpointAddress;

	libusb_device_handle *handle;
	libusb_open(device, &handle);

	// bool reatach = false;

	// if(libusb_kernel_driver_active(handle, interface_num) == 1){
	// 	libusb_detach_kernel_driver(handle,interface_num);
	// 	reatach = true;
	// }

	// libusb_set_configuration(handle, 1);
	

	// int claim = libusb_claim_interface(handle, interface_num);
	// if(claim == 0){
	// 	unsigned char data[128];
	// 	int bulk_transfer = libusb_bulk_transfer(handle, LIBUSB_ENDPOINT_IN, data, sizeof(data), NULL, 0 );
	// 	if(bulk_transfer == 0){
	// 		cout << data << endl;
	// 	}else{
	// 		cout << "bulk transfer error: " << bulk_transfer << ": " << libusb_error_name(bulk_transfer) << ": " << libusb_strerror(LIBUSB_ERROR_IO)<< endl;
	// 	}
	// }

	// if(reatach){
	// 	int err = libusb_attach_kernel_driver(handle, interface_num);
	// 	cout << "Kernel reattach error: " << err << ": " << libusb_error_name(err) << ": " << libusb_strerror(LIBUSB_ERROR_BUSY) << endl;
	// }

	// if (libusb_kernel_driver_active(handle, interface_num) == 1){
	// 	cout << "reattached" << endl;
	// }else{
	// 	cout << "kernel reattach error" << endl;
	// }

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














// int choose_interface(libusb_device *dev){
// 	libusb_config_descriptor *config;
// 	libusb_get_config_descriptor(dev, 0, &config);

// 	cout <<"Interfaces: " << endl;

// 	const libusb_interface *inter;

// 	for(int i  = 0; i < (int)config->bNumInterfaces; i++){
// 		inter = &config->interface[i];
//  		cout<<"Interface "<< i << ": " <<"Number of Alternate Settings: "<<inter->num_altsetting<<endl;
// 	}

// 	cout << "Please select an Interface using its number" << endl;

// 	string input;
// 	getline(cin, input);
// 	int returnVal = stoi(input);

// 	return returnVal;
// }


// int choose_alternate_setting(libusb_device *dev, int interface){

// 	libusb_config_descriptor *config;
// 	libusb_get_config_descriptor(dev, 0, &config);

// 	cout << config->bConfigurationValue << endl;

// 	const libusb_interface *inter;
// 	const libusb_interface_descriptor *interdesc;

// 	inter = &config->interface[interface];

// 	for(int i = 0; i < inter->num_altsetting; i++){
// 		interdesc = &inter->altsetting[i];
// 		cout << "Alternate Setting " << i << ": ";
// 		cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
// 		cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<< endl;
// 	}

// 	cout << "Please Choose an Alternate Setting Number: " << endl;

// 	string input;
// 	getline(cin, input);
// 	int returnVal = stoi(input);

// 	return returnVal;
// }

// int endpoint_selector(libusb_device *dev, int interface, int alternate_set){
// 	libusb_config_descriptor *config;
//  	libusb_get_config_descriptor(dev, 0, &config);

// 	const libusb_interface *inter;
// 	const libusb_interface_descriptor *interdesc;
// 	const libusb_endpoint_descriptor *epdesc;

// 	inter = &config->interface[interface];
// 	interdesc = &inter->altsetting[alternate_set];

// 	cout << "Endpoints: " << endl;

// 	for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
// 		epdesc = &interdesc->endpoint[k];
// 		cout << "Endpoint " << k << ": ";
// 		cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
// 		cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<endl;
// 	}

// 	cout << "Please Select an Endpoint Using its Number:" << endl;
	
// 	string input;
// 	getline(cin, input);

// 	int returnVal = stoi(input);

// 	return returnVal;
// }


//Was started on line 24 in the main function
//cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
	//if(cnt < 0) {
		//cout<<"Get Device Error"<<endl; //there was an error
	//}
	//cout<<cnt<<" Devices in list."<<endl; //print total number of usb devices
		//ssize_t i; //for iterating through the list
	// for(i = 0; i < cnt; i++) {
	// 			printdev(devs[i]); //print specs of this device
	// 	}
	// 	libusb_free_device_list(devs, 1); //free the list, unref the devices in it

// void printdev(libusb_device *dev) {
// 	libusb_device_descriptor desc;
// 	int r = libusb_get_device_descriptor(dev, &desc);
// 	if (r < 0) {
// 		cout<<"failed to get device descriptor"<<endl;
// 		return;
// 	}
// 	cout<<"Number of possible configurations: "<<(int)desc.bNumConfigurations<<"  ";
// 	cout<<"Device Class: "<<(int)desc.bDeviceClass<<"  ";
// 	cout<<"VendorID: "<<desc.idVendor<<"  ";
// 	cout<<"ProductID: "<<desc.idProduct<<endl;

// 	libusb_config_descriptor *config;
// 	libusb_get_config_descriptor(dev, 0, &config);

// 	cout<<"Interfaces: "<<(int)config->bNumInterfaces<<" ||| ";

// 	const libusb_interface *inter;
// 	const libusb_interface_descriptor *interdesc;
// 	const libusb_endpoint_descriptor *epdesc;

// 	for(int i=0; i<(int)config->bNumInterfaces; i++) {
// 		inter = &config->interface[i];
// 		cout<<"Number of alternate settings: "<<inter->num_altsetting<<endl;

// 		for(int j=0; j<inter->num_altsetting; j++) {
// 			interdesc = &inter->altsetting[j];
// 			cout<<"Interface Number: "<<(int)interdesc->bInterfaceNumber<<" | ";
// 			cout<<"Number of endpoints: "<<(int)interdesc->bNumEndpoints<< endl;

// 			for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
// 				epdesc = &interdesc->endpoint[k];
// 				cout<<"Descriptor Type: "<<(int)epdesc->bDescriptorType<<" | ";
// 				cout<<"EP Address: "<<(int)epdesc->bEndpointAddress<<endl;
// 			}
// 		}
// 	}
// 	cout<<endl<<endl<<endl;
// 	libusb_free_config_descriptor(config);
// }