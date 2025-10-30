; Put a string in SI and call this to run it

print_string:
	pusha			; Push all register values to the stack
	mov ah, 0x0e	; scrolling teletype BIOS routine

.next_char:
	lodsb			; Load byte from DS:SI into AL, then SI++
	cmp al, 0 		; Is it null terminator?
	je done		; if yes, printing done
	int 0x10		; else, print the charac
	jmp .next_char  ; continue loop

done:
	popa			; Restore original register values
	ret
