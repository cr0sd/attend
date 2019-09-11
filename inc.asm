; nasm
section .data
global LICENSE_STRING
LICENSE_STRING:
	incbin "license.txt"
db 0
