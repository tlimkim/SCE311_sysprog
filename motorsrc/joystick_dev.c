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

#define GPIO 120
#define DEV_NAME "joystick_dev"
#define DEV_NUM 241

MODULE_LICENSE("GPL");
static ssize_t joy_stick_read(struct file * file, char * buf, size_t length, loff_t * ofs)
{
  printk("READ joy_stick_dev called\n");
  int stat[3];
   stat[0] = gpio_get_value(GPIO + 0);
   stat[1] = gpio_get_value(GPIO + 1);
   stat[2] = gpio_get_value(GPIO + 2);
  //printk("READ stat: %d\n",stat);
  return copy_to_user(buf, &stat, sizeof(stat));
  //ret[1] = copy_to_user(buf, &stat[1], sizeof(stat[1]));
  //ret[2] = copy_to_user(buf, &stat[2], sizeof(stat[2]));

}

int joy_stick_open(struct inode *pinode, struct file *pfile) 
{
  printk("OPEN joy_stick_dev\n"); 
  gpio_request(GPIO + 0, "GPIO");
  gpio_request(GPIO + 1, "GPIO");
  gpio_request(GPIO + 2, "GPIO");
 
  gpio_direction_input(GPIO + 0);
  gpio_direction_input(GPIO + 1);
  gpio_direction_input(GPIO + 2);

  return 0;
}

int joy_stick_close(struct inode *pinode, struct file *pfile)
{
  printk("RELEASE joy_stick_dev\n");
  
  gpio_unexport(GPIO + 0);
  gpio_unexport(GPIO + 1);
  gpio_unexport(GPIO + 2);

  gpio_free(GPIO +0);
  gpio_free(GPIO +1);
  gpio_free(GPIO +2);

  return 0;
}


static struct file_operations fop={
  .owner = THIS_MODULE,
  .open = joy_stick_open,
  .release = joy_stick_close,
  .read = joy_stick_read,
};

int __init joy_stick_init(void){
  printk("INIT joy_stick_dev\n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  //printk("REGR result: %d\n",result);
  return 0;
}

void __exit joy_stick_exit(void){
  printk("EXIT joy_stick_dev\n");
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(joy_stick_init);
module_exit(joy_stick_exit);