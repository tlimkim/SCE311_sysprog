/*
 * Developed by Jiwon Jeon
 *
 * pir_dev.c: device driver code for PIR.
 */
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>

#include <linux/uaccess.h>
#include <linux/mutex.h>
#define GPIO 21
#define DEV_NAME "pir_dev"
#define DEV_NUM 241

MODULE_LICENSE("GPL");
static ssize_t pir_read(struct file * file, char * buf, size_t length, loff_t * ofs)
{
  printk("READ pir_dev called\n");
  int ret;
  int stat;
  stat = gpio_get_value(GPIO);
  printk("READ stat: %d\n",stat);
  ret = copy_to_user(buf, &stat, sizeof(stat));
  return (ret == 0) ? sizeof(stat) : ret;
}

int pir_open(struct inode *pinode, struct file *pfile) 
{
  int ret;
  printk("OPEN pir_dev\n"); 
  ret = gpio_request(GPIO, "GPIO");
  if (ret) 
    printk("### FAILED REQUEST gpio(%d) error: %d",GPIO,ret);
  else
    gpio_direction_input(GPIO);

  return 0;
}

int pir_close(struct inode *pinode, struct file *pfile)
{
  printk("RELEASE pir_dev\n");
  return 0;
}


static struct file_operations fop={
  .owner = THIS_MODULE,
  .open = pir_open,
  .release = pir_close,
  .read = pir_read,
};

int __init pir_init(void){
  printk("INIT pir_dev\n");
  int result=register_chrdev(DEV_NUM, DEV_NAME, &fop);
  printk("REGR result: %d\n",result);
  return 0;
}

void __exit pir_exit(void){
  printk("EXIT pir_dev\n");
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(pir_init);
module_exit(pir_exit);
