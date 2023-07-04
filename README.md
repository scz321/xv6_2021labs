## syscall分支已完成

###### 测试信息：

== Test trace 32 grep ==
$ make qemu-gdb
trace 32 grep: OK (2.1s)
== Test trace all grep ==
$ make qemu-gdb
trace all grep: OK (0.3s)
== Test trace nothing ==
$ make qemu-gdb
trace nothing: OK (1.0s)
== Test trace children ==
$ make qemu-gdb
trace children: OK (8.2s)
== Test sysinfotest ==
$ make qemu-gdb
sysinfotest: OK (1.0s)
    (Old xv6.out.sysinfotest failure log removed)
== Test time ==
time: OK
Score: 35/35
