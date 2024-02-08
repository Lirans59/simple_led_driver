#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/errno.h>

#define GPIO_BASE           0xfe200000 // 
#define GPFSEL1_OFFSET      0x04        // GPIO function select register (for pins 0-9)
#define GPSET0_OFFSET       0x1c        // GPIO output set register
#define GPCLR0_OFFSET       0x28        // GPIO output clear register
#define GPLEV0_OFFSET       0x34        // GPIO output clear register



static dev_t first;
static struct cdev c_dev;
static struct class *cl;
static volatile void __iomem *gpfsel1;           // Physical address mapped to vitual address
static volatile void __iomem *gpset0;            // Physical address mapped to vitual address
static volatile void __iomem *gpclear0;
static volatile void __iomem *gplev0;

static int my_open(struct inode *i, struct file *f)
{
	return 0;
}

ssize_t my_write (struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    char input;
    u32 reg;

    if( copy_from_user(&input, buff, 1) )
    {
        printk(KERN_INFO "copy_from_user failed\n");
        return -1;
    }
    if(input != '0' && input != '1')
    {
        printk(KERN_INFO "Invalid input\n");
        return -EINVAL;
    }
    printk(KERN_INFO "input =  %c\n", input);
    if(input == '1')
    {
        /* gpset0 */
        reg = readl(gpset0);
        printk(KERN_INFO "gpset0 = %x\n", reg);
        reg |= (1 << 17);
        writel(reg, gpset0);
        printk(KERN_INFO "gpset0 = %x\n", readl(gpset0));
        printk(KERN_INFO "gplev0 = %x\n", readl(gplev0));
    }
    else
    {
        /* gpclear0 */
        reg = readl(gpclear0);
        printk(KERN_INFO "gpclear0 = %x\n", reg);
        reg |= (1 << 17);
        writel(reg, gpclear0);
        printk(KERN_INFO "gpclear0 = %x\n", readl(gpclear0));
        printk(KERN_INFO "gplev0 = %x\n", readl(gplev0));
    }
    
    
    return 0;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
	.open = my_open,
	// .release = my_close,
	// .read = my_read,
	.write = my_write
};

static int __init led_init(void)
{
    u32 reg;

    printk(KERN_INFO "Loading simple_led module...\n");

    if (alloc_chrdev_region(&first, 0, 1, "LED") < 0) {
        printk(KERN_INFO "Allocating char dev region failed\n");
        goto alloc_chrdev_region_fail;
    }
    if (IS_ERR(cl = class_create(THIS_MODULE, "LED"))) {
        printk(KERN_INFO "Class creating failed\n");
        goto class_create_fail;
    }
    if (IS_ERR(device_create(cl, NULL, first, NULL, "LED"))) {
        printk(KERN_INFO "Device Creating failed\n");
        goto device_create_fail;
    }
    cdev_init(&c_dev, &led_fops);
    if (cdev_add(&c_dev, first ,1) < 0) {
        printk(KERN_INFO "Adding device failed\n");
        goto cdev_add_fail;
    }
    // --------------------------------------------------------------------

    printk(KERN_INFO "ioremap\n");
    gpfsel1 = (volatile void*)ioremap(GPIO_BASE + GPFSEL1_OFFSET, 4);
    if(gpfsel1 == NULL) {
        printk(KERN_INFO "ioremap failed\n");
        return 1;
    }

    gpset0 = (volatile void*)ioremap(GPIO_BASE + GPSET0_OFFSET, 4);
    if(gpset0 == NULL) {
        printk(KERN_INFO "ioremap failed\n");
        return 1;
    }
    
    gpclear0 = (volatile void*)ioremap(GPIO_BASE + GPCLR0_OFFSET, 4);
    if(gpclear0 == NULL) {
        printk(KERN_INFO "ioremap failed\n");
        return 1;
    }

    gplev0 = (volatile void*)ioremap(GPIO_BASE + GPLEV0_OFFSET, 4);
    if(gplev0 == NULL) {
        printk(KERN_INFO "ioremap failed\n");
        return 1;
    }

    /* gpsel1 */
    reg = readl(gpfsel1);
    reg |= (1 << 21);                   // Set bit 21
    reg &= ~((1 << 22) + (1 << 23));    // Clear bits 22-23
    writel(reg, gpfsel1); 
    printk(KERN_INFO " --- gpfsel1 = %x ---\n", readl(gpfsel1));


    printk(KERN_INFO " --- gplev0 = %x ---\n", readl(gplev0));

    printk(KERN_INFO "LED module loaded!\n");
    return 0;

cdev_add_fail:
    device_destroy(cl, first);
device_create_fail:
    class_destroy(cl);
class_create_fail:
    unregister_chrdev_region(first, 1);
alloc_chrdev_region_fail:
    return -1;
}

static void __exit led_exit(void)
{
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    iounmap(gpfsel1);
    iounmap(gpset0);
    iounmap(gpclear0);

    printk(KERN_INFO "LED module unloaded\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Liran Shukrun");
MODULE_DESCRIPTION("LED Character Driver");