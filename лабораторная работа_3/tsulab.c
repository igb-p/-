#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h> 

static int __init example_init(void) {
 printk(KERN_INFO "Welcome to the Tomsk State University\n");
 return 0;
}
static void __exit example_exit(void) {
 printk(KERN_INFO "Tomsk State University forever!\n");
}
module_init(example_init);
module_exit(example_exit);

MODULE_LICENSE("GPL");




