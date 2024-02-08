#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbd97005b, "module_layout" },
	{ 0xedc03953, "iounmap" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xafe18e20, "class_destroy" },
	{ 0x1d37eeed, "ioremap" },
	{ 0xc65b2c7a, "device_destroy" },
	{ 0x4a91e5dd, "cdev_add" },
	{ 0xc72c11da, "cdev_init" },
	{ 0xfada38fe, "device_create" },
	{ 0xd9379699, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x86332725, "__stack_chk_fail" },
	{ 0x5f754e5a, "memset" },
	{ 0xc5850110, "printk" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D29A94E290C8D7C49676F61");
