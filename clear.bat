@echo off
rmdir project\win\nf\.vs /q/s
rmdir project\win\nf\x64 /q/s
rmdir project\win\nf\Debug /q/s
rmdir project\win\nf\Release /q/s

rmdir project\win\nf\nfClient\x64 /q/s
rmdir project\win\nf\nfClient\Debug /q/s
rmdir project\win\nf\nfClient\Release /q/s

rmdir project\win\nf\nfServer\x64 /q/s
rmdir project\win\nf\nfServer\Debug /q/s
rmdir project\win\nf\nfServer\Release /q/s

del project\win\nf\*.log /q/s
del project\win\nf\*.db /q/s