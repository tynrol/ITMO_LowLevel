section .text


global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy


; Принимает код возврата и завершает текущий процесс
exit:
    xor rax, rax
    mov rax, 60
    syscall


; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
    .loop:
        cmp byte[rdi+rax], 0
        je .exit
        inc rax
        jmp .loop
    .exit:
   	ret


; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    xor rax, rax
    mov rsi, rdi
    call string_length
    mov rdx, rax ;длина
    mov rdi, 1	 ;дескриптор
    mov rax, 1	 ;код записи
    syscall
    ret


; Принимает код символа и выводит его в stdout
print_char:
    xor rax, rax
    push rdi   ;используем стек, чтобы поместить в rsi указатель(как в print_string) 
    mov rsi, rsp
    mov rdx, 1 ;длина 
    mov rdi, 1 ;дескриптор
    mov rax, 1 ;код write
    syscall
    pop rdi
    ret



; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov rdi, 10
    call print_char
    ret


; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    xor rax, rax
    xor rcx, rcx
    mov rax, rdi
    mov rdi, 10 ; делитель
    mov r8, rsp ; сохр. нач. SP	
    push 0 ; обозначаем конец 
    .loop:
	xor rdx, rdx
	div rdi
	add rdx, '0' ; перевод в ascii
	dec rsp
	mov byte[rsp], dl ; записываем цифру в стек
	test rax, rax 
	jnz .loop ; если 0 -> вывод
    mov rdi, rsp
    call print_string
    mov rsp, r8
    ret



; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    test rdi, rdi
    jns print_uint
    push rdi
    mov rdi, '-'
    call print_char
    pop rdi
    neg rdi
    jmp print_uint


; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    .loop:
	mov al, byte[rdi+rcx]
        mov dl, byte[rsi+rcx]

        cmp al, dl
        jne .failure

        cmp al, 0
        je .success

	inc rcx
        jmp .loop
    .success:
        mov rax, 1
        ret
    .failure:
	mov rax, 0
        ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
	push rdi
	push rsi
	xor rax, rax
	mov rdi, 0 ; дескриптор
	mov rdx, 1 ; длина
	mov rsi, rsp ; исп. место под стек
	syscall

	test rax, rax
	jnz .end
	mov byte[rsi], 0

	.end:
		mov al, byte[rsi]
		pop rsi
		pop rdi
		ret


; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
read_word:
	xor r9, r9
	xor rax, rax
	.loop:
		cmp r9, rsi
		jg .failure

		push rdx
		push rdi
		push rsi

		call read_char

		pop rsi
		pop rdi
		pop rdx

		cmp al, 0x20
		je .success
		cmp al, 0xA
		je .success
		cmp al, 0x9
		je .success
		cmp al, 0
		je .end

		mov byte[rdi+r9], al
		inc r9
		jmp .loop
	.failure:
		mov rax, 0
		ret
	.success:
		test r9, r9
		jz .loop
	.end:
		mov byte[rdi+r9], 0
		mov rax, rdi
		mov rdx, r9
		ret


; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
 	xor rdx, rdx
	xor rax, rax
	xor rcx, rcx
	mov r9, 10
	.loop:
		xor r8, r8
		cmp byte[rdi+rcx], '0'
		jl .exit
		cmp byte[rdi+rcx], '9'
		jg .exit

		mov r8b, byte[rdi+rcx]
		sub r8, '0'
		mul r9
		add rax, r8

		inc rcx
		jmp .loop
	.exit:
		mov rdx, rcx
		ret



; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
	xor rdx, rdx
	xor rax, rax
	xor rcx, rcx
	push rdi
	.sign:
		cmp byte[rdi+rcx], '-'
		je .neg

		call parse_uint
		jmp .exit
	.neg:
		inc rdi
		call parse_uint
		neg rax
		inc rdx
	.exit:
		pop rdi
		ret


; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
	call string_length
	cmp rdx, rax
	jle .noSpace

	.loop:
	xor rax, rax
	mov al, byte[rdi]
	mov byte[rsi], al
	inc rdi
	inc rsi
	cmp al, 0
	jne .loop
	ret

	.noSpace:
	mov rax, 0
	ret


