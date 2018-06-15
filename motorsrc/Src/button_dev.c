#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>                 // Required for the GPIO functions
#include <linux/interrupt.h>            // Required for the IRQ code
#include <linux/kmod.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

#define DEV_NAME "button_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");
 
static unsigned int gpioButton = 23;   // button gpio pin port

/*
 * Open the gpio 23 port to the kernel with the gpio open function, and set it to input.
 */
int gpio_open(struct inode* pinode, struct file* pfile){
   printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST Baby_Button\n");
   gpio_request(gpioButton, "sysfs");       // Set up the gpioButton
   gpio_direction_input(gpioButton);        // Set the button GPIO to be an input
   return 0;
}
/*
 * When you are finished, unport the gpio port and use the free function.
 */
int gpio_close(struct inode* pinode, struct file* pfile){
   printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton));
   printk(KERN_INFO "GPIO_TEST: The button was pressed %d times\n", numberPresses);
   gpio_unexport(gpioButton);               // Unexport the Button GPIO
   gpio_free(gpioButton);                   // Free the Button GPIO
   printk(KERN_INFO "GPIO_TEST: Goodbye from the Baby!\n");
   return 0;
}

/*
 * It reads the gpio value and passes the read value to the code running in the user application through the copy to user function.
 */
ssize_t simple_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
	int ret = gpio_get_value(gpioButton);
	if(ret == 0){
		printk("button pressed val: %d\n", ret);
		return copy_to_user(buffer, &ret , sizeof(ret));
	}
	else return 0;

}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = gpio_open,
  .read = simple_read,
  .release = gpio_close,
  
};

int __init Button_init(void)
{
  printk("Init baby Button & Camera \n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  return 0;
}

void __exit Button_exit(void)
{
  printk("Exit baby Button & Camera \n");
  unregister_chrdev(DEV_NUM, DEV_NAME);
}


module_init(Button_init);
module_exit(Button_exit);
