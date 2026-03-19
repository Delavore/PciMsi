#define BAR_SPACE 0x0

struct Minpci_State {
	PCIDevice parent_dev;
	MemoryRegion mmio;
	
	uint64_t bar0_space;	
};



static void minpci_write(void *opaque, hwaddr addr, uint64_t val, unsigned size) {
	struct Minpci_State *mpci = opaque;
	
	if (addr == BAR_SPACE) {
		mpci->bar0_space = val;
		pr_info("WRITE");
		return;	
	}
	
	pr_info("INVALID ADDR");
	return;
}

static uint64_t minpci_read(void *opaque, hwaddr, unsigned size) {
	struct Minpci_State *mpci = opaque;
	uint64_t val = ~0ULL;
	
	if (addr == BAR_SPACE) {
		val = mpci->bar0_space;	
	}

	return val;
}


static void minpci_class_init(ObjectClass *class, const void *data) {
	DeviceClass *dc = DEVICE_CLASS(class);
	PCIDeviceClass *k = PCI_DEVICE_CLASS(class);

	k->realize = minpci_realize;
	k->exit = minpci_uninit;
	k->vendor_id = PCI_VENDOR_ID_STUB;
	k->device_id = PCI_DEVICE_ID_STUB;
	k->revision = 0x00;
	k->class_id = PCI_CLASS_OTHER;
	
	set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

