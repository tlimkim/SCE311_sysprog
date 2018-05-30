#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>

#define GPIO1 26
#define DEV_NAME "led_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

int led_open(struct inode *pinode, struct file *pfile)
{
  printk("Open LED \n");
  gpio_request(GPIO1, "GPIO1");
  gpio_direction_output(GPIO1, 1);

  return 0;
}

int led_close(struct inode *pinode, struct file *pfile)
{
  printk("Close LED \n");
  return 0;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = led_open,
  .release = led_close,
};

int __init led_init(void)
{
  printk("Init LED \n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);
  return 0;
}

void __exit led_exit(void)
{
  printk("Exit LED \n");
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(led_init);
module_exit(led_exit);

