.model small
.stack 1000
.data
        old dd 0 		
		count dw 0;согласный
        ten dw 10

.code
.486
        
		counts dw 0; согланые
 
new_handle proc   
      
        push ds si es di dx cx bx ax ;заносим в стек значения регистров перед выполнением нашего обработчика прерываний
        
        xor ax, ax ; обнуляем регистр
		
        in al, 60h        ;Получаем скан-код символа        
		
		cmp al, 10h        ;q
        je new_handler
        cmp al, 11h        ;w
        je new_handler
        cmp al, 13h        ;r
        je new_handler
        cmp al, 14h        ;t
        je new_handler
        cmp al, 19h        ;p
        je new_handler
		cmp al, 1Fh        ;s
        je new_handler
        cmp al, 20h        ;d
        je new_handler
        cmp al, 21h        ;f
        je new_handler
        cmp al, 22h        ;g
        je new_handler
        cmp al, 23h        ;h
        je new_handler
		cmp al, 24h        ;j
        je new_handler
        cmp al, 25h        ;k
        je new_handler
        cmp al, 26h        ;l
        je new_handler
        cmp al, 2Ch        ;z
        je new_handler
        cmp al, 2Dh        ;x
        je new_handler
		cmp al, 2Eh        ;c
        je new_handler
        cmp al, 2Fh        ;v
        je new_handler
        cmp al, 30h        ;b
        je new_handler
        cmp al, 31h        ;n
        je new_handler
        cmp al, 32h        ;m
        je new_handler		
        
		cmp al,3Ch ;F2
		je output
		
        jmp old_handler  
		
		
		output:
			mov AX,count ;Выводимое число в регисте AX. гласные
			push   -1  ;Сохраним признак конца числа
			mov    CX,10   ;Делим на 10
    
		l3: 
			mov DX,0   ;Очистим регистр DX
			div    CX  ;Делим 
			push DX    ;Сохраним цифру
			cmp  AX,0  ;Остался 0?
			jne    l3  ;нет -> продолжим
			mov    AH,2h
		l4:
			pop    DX  ;Восстановим цифру
			cmp    DX,-1   ;Дошли до конца -> выход
			je exit
			add    DL,'0'  ;Преобразуем число в цифру
			int    21h ;Выведем цифру на экран
			jmp    l4  ;И продолжим			
		
        new_handler: 

                inc count ;инкремент счетчика с каждой согласной буквой
                jmp exit
        
        old_handler: 
                pop ax bx cx dx di es si ds                       
                jmp dword ptr cs:old        ;вызов стандартного обработчика прерывания
                xor ax, ax
                mov al, 20h
                out 20h, al 		
                jmp exit
        exit:
		    		
                xor ax, ax	
				mov ax,5
                mov al, 20h
                out 20h, al 		
                pop ax bx cx dx di es si ds ;восстановление регистров перед выходом из нашего обработчика прерываний
				
        iret
new_handle endp

new_end:
 
start:
        mov ax, @data
        mov ds, ax
		
		
		
        cli ;сброс флага IF 	
        pushf 
		
        push 0        ;перебрасываем значение 0 в DS
        pop ds
        mov eax, ds:[09h*4] 
        mov cs:[old], eax ;сохранение системного обработчика
        		
        mov ax, cs
        shl eax, 16
        mov ax, offset new_handle
        mov ds:[09h*4], eax ;запись нового обработчика
		
        sti ;Установка флага IF     
		
		
        xor ax, ax
        mov ah, 31h
        INT 21H 
        MOV DX, (New_end - @code        + 10FH) / 16 ;вычисление размера резидентной части в параграфах(16 байт)
end start