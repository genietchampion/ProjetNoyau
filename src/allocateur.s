;/**************************************************************************//**
; * @file     allocateur.s
; * @brief    
; *           
; * @version  V1.0
; * @date     
; *
; * @note
; * 
; *
; * @par
; * 
; *
; * @par
; * 
; *
; ******************************************************************************/

; constantes

Stack_Size      EQU     0x00000040

controleThreadNP	EQU	0x1					; valeur pour controle avec mode thread non privilégié
											; pour la pile c'est lors de retour d'exceprion qu'elle est positionnée
xPSRInitial 	EQU		0x01000000			; xPSR avec juste Thumb à 1, le reste est à 0 (comme suite à un reset)

LRThreadPSP		EQU		0xFFFFFFFD			; valeur de la pseudo @ de retour pour une exception en revenant en mode
											; thread avec utilisation de la pile PSP
											
; area => ALIGN implicit de 4 octets
; ici area data standard initialisé

                AREA    |.data|, DATA, READWRITE

; les piles sont à aligner sur 8 octets en cas de gestion des nombres flottants, comme ce n'est pas le cas avec
; notre projet, nous ne devrions pas être embeté
; cependant il est toujours préférable de respecter les conventions afin d'éviter tous problèmes

; SPACE ne gère pas l'alignement et les area sont pas défaut sur des adresses doublement paires, on va donc forcer
; l'alignement sur une adresse triplement paires

				ALIGN 8

; Pile PSP pour demoRestitution

				SPACE   8*Stack_Size		; on multiplie la pile par 8, on est sur de conserver l'alignement pour le début
											; de la pile
Pile_PSP									; rappel le début de la pile est en bas

; Pile MSP pour demoRestitution

				SPACE   8*Stack_Size
Pile_MSP									; rappel le début de la pile est en bas


; DCD permet l'initialisation et la réservation de 4 octets, il est aligné sur une frontière doublement paire (modulo 4 donc)

; Initialisation en dur d'un contexte à restituer
				
etatCalcul		DCD		controleThreadNP	; controle
				DCD		4					; R4
				DCD		5					; R5
				DCD		6					; ...
				DCD		7
				DCD		8
				DCD		9
				DCD		10
				DCD		11					; R11
				DCD		Pile_PSP			; PSP
				DCD		Pile_MSP			; MSP
				DCD		LRThreadPSP			; LR en mode handler

; area => ALIGN implice de 4 octets
; zone de code

                AREA    |.text|, CODE, READONLY

				PRESERVE8					; le C souhaite une pile sur une @ triplement double, sans cette option 
											; l'assembleur lance des warning comme on s'interface avec le langage C
											; mais attention, si on veut préserver la pile sur une @ triplement double
											; c'est à nous programmeur de faire ce qu'il faut
								
				
demoRestitution	PROC
 				EXPORT  demoRestitution

; fct qui va faire une création de processus/restitution de contexte à partir d'une structure etatCalcul et
; de piles statiques

; on est :
;  en mode handler
;  les IT sont désactivé => indivisible/initerruptible
;  la structure à restituer est dans etatCalcul
;  les registres du processeur sont libres et disponibles
;  la pile MSP est valide


; à vous d'écrire le code ici!!!!!!!!!!!!!!!!!!!!!!!!

				LDR R0,=etatCalcul
				LDR R1,[R0],#4
				MSR CONTROL,R1
				LDR R1,[R0],#4
				MOV R4,R1
				LDR R1,[R0],#4
				MOV R5,R1
				LDR R1,[R0],#4
				MOV R6,R1
				LDR R1,[R0],#4
				MOV R7,R1
				LDR R1,[R0],#4
				MOV R8,R1
				LDR R1,[R0],#4
				MOV R9,R1
				LDR R1,[R0],#4
				MOV R10,R1
				LDR R1,[R0],#4
				MOV R11,R1
				LDR R1,[R0],#4
				MSR PSP,R1
				ISB
				LDR R1,[R0],#4
				MSR MSP,R1
				ISB
				LDR R1,[R0],#4
				MOV LR,R1
  
				BX LR

; on ne doit pas passer par ce code, par sécurité on met une boucle

				B	.
				
				ENDP

				END

