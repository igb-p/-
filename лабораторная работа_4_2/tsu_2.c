#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h> 
#include<linux/proc_fs.h>
#include<linux/uaccess.h>
#include <linux/version.h> 

#define procfs_name "tsu_2"
static struct proc_dir_entry *our_proc_file = NULL;

int count=0;
char s[64];
ssize_t len;
static ssize_t procfile_read(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t* offset) 
{
   static bool fl = 0;
   if(fl)
   {
      fl=0;
      return 0;
   }
   count++;
   if(count%2==0) {len=sprintf(s, "1 - even number of times");}
   else {len=sprintf(s, "0 - odd number of times");}
   copy_to_user(buffer, s, len); 
   pr_info("procfile read %s\n",file_pointer->f_path.dentry->d_name.name);
   fl=1;
   return len;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops proc_file_fops = 
{
   .proc_read = procfile_read,
};
#else
static const struct file_operations proc_file_fops =
{
   .read = procfile_read,
};
#endif


static int __init procfs1_init(void)
{
   our_proc_file = proc_create(
   procfs_name, 0644, NULL, &proc_file_fops);
   return 0;
}
static void __exit procfs1_exit(void)
{
   proc_remove(our_proc_file);
   pr_info("/proc/%s removed\n", procfs_name);
} 
module_init(procfs1_init);
module_exit(procfs1_exit);
MODULE_LICENSE("GPL");
