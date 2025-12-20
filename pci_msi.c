struct PmsiState {
	PCIDevice parent_dev;

	MemoryRegion mmio;
};


static void pmsi_class_init(ObjectClass *class, const void *data) {
	DeviceClass *dc = DEVICE_CLASS(class);
	PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

	k->realize = pmsi_realize;
	k->exit = pmsi_uninit;
	k->vendor_id = PCI_VENDOR_ID_STUB;
	k->device_id = PCI_DEVICE_ID_STUB;
	k->revision = 0x00;
	k->class_id = PCI_CLASS_OTHER;
	
	set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}
