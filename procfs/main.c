#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

/* Demonstrate how to use the single_open to write the proc entry */

MODULE_LICENSE("Dual BSD/GPL");

#define PROCNAME "sampleQoo"

static unsigned int g_num;

static int proc_sample_get(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", g_num);
	return 0;
}

ssize_t proc_sample_set(struct file *file, const char __user *buffer, size_t count, loff_t *pos)
{
	char cmd[32] = {0};
	u8 result = 0;

	if (!copy_from_user(cmd, buffer, count)) {
		sscanf(cmd, "%hhx", &result);
		printk(KERN_INFO "%s called cmd is %hhx\n", __func__, result);

		if (0 == result) {
			g_num = 0;
		} else {
			g_num = 1;
		}
	}

	return count;
}

static int sample_proc_open(struct inode *inode, struct file *file)
{
	/*return seq_open(file, &sample_seq_op);*/
	/*return single_open(file, &proc_sample_get, PDE((inode))->parent->data);*/
	/*ssize_t a = PDE(inode)->data;*/
	ssize_t a = (ssize_t)PDE_DATA(inode);
	printk(KERN_INFO "value is :%lu  (%s)\n", a, __func__);
	return single_open(file, &proc_sample_get, NULL);
}

/* procfs handler */
static struct file_operations sample_operations = {
	.owner = THIS_MODULE,
	.open = sample_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
	.write = proc_sample_set,
};

static int sample_init(void)
{
	struct proc_dir_entry *entry;

	/* add /proc */
	/*entry = create_proc_entry(PROCNAME, S_IRUGO | S_IWUGO, NULL);*/
	ssize_t a = 5;
	entry = proc_create_data(PROCNAME, S_IRUGO | S_IWUGO, NULL, &sample_operations, (void *)a);
	/*if (entry)*/
	/*entry->proc_fops = &sample_operations;*/

	printk("driver loaded\n");

	return 0;
}

static void sample_exit(void)
{
	remove_proc_entry(PROCNAME, NULL);

	printk(KERN_ALERT "driver unloaded\n");
}

module_init(sample_init);
module_exit(sample_exit);

