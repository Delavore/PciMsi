#define BAR0_SPACE 0x0
#define PCI_VENDOR_ID_STUB 0x10
#define PCI_DEVICE_ID_STUB 0x11

struct Minpci_State {
	PCIDevice parent_dev;
	MemoryRegion mmio;
	
	uint32_t bar0_space;	
};

DECLARE_INSTANCE_CHECKER(struct Minpci_State, MINPCIDEV, TYPE_PCI_CUSTOM_DEVICE)

static void minpci_write(void *opaque, hwaddr addr, uint32_t val, unsigned size) {
	struct Minpci_State *mpci = opaque;
	
	if (addr == BAR_SPACE) {
		mpci->bar0_space = val;
		pr_info("WRITE");
		return;	
	}
	
	pr_info("INVALID ADDR");
	return;
}

static uint32_t minpci_read(void *opaque, hwaddr addr, unsigned size) {
	struct Minpci_State *mpci = opaque;
	uint32_t val = ~0ULL;
	
	if (addr == BAR0_SPACE) {
		val = mpci->bar0_space;	
	}

	return val;
}


static const MemoryRegionOps bar0_mmio_ops {
	.read = minpci_read,
	.write = minpci_write, 
	.endianness = DEVICE_NATIVE_ENDIAN,

	/*
	.valid = {
		.min_access_size = 4,
		.max_access_size = 4,
	},
	.impl = {
		.min_access_size = 4,
		.max_access_size = 4,
	}
	*/
}

static void minpci_realize(PCIDevice *pdev, Error **errp) {
	struct Minpci_State *mpci = MINPCIDEV(pdev);


	mpci->bar0_space = 80;  // test value
	
	memory_region_init_io(&mpci->mmio, OBJECT(mpci) , &bar0_mmio_ops, mpci, "our_register", 4);
	pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &mpci->mmio);
}

static void minpci_uninit(PCIDevice *pdev) {
	return;
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
