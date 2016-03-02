;/**************************************************************************//**
; * @file     initNoyau.s
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

Stack_Size      EQU     0x00000200			; taille de notre pile (pas besoin d'une grosse pile)


; a priori les piles sont � aligner sur 8 octets en cas de gestion des nombres flottants, comme ce n'est pas le cas avec
; notre projet, nous ne devrions pas �tre embet� mais autant ne pas prendre de risque.... puisque l'on peut
; on utilise ici une area d�di� aux deux piles, c'est primordiale pour la pile MSP, puisque dans le startup, avant l'appel
; � main, on appelle une proc�dure de la biblioth�que __main qui s'occupe entre autre de l'initialisation des variables et
; des zones, notamment l'area .data va �tre initialis� � 0 (section data ZI) sauf pour les variables initialis�es (recopie
; d'une zone ROM vers une zone RAM)

; la pile MSP est normalement d�fini dans le fichier startup, sur une fronti�re de 8 et dans une area sp�cifique,
; on la rapatrier ici d�s fois que l'on souhaite modifier quelque chose

				AREA    STACK, NOINIT, READWRITE, ALIGN=3
			   
				EXPORT __initial_sp
Stack_Mem       SPACE   Stack_Size			; r�servation d'espace
__initial_sp

; Gestion de la pile PSP initiale :
;  Comme nous n'en avons pas besoin, je commente le code, si besoin on peut le r�activer

; on r�aligne d�s fois que la taille en soit pas un multiple de 8

;				ALIGN 8

; espace pour la pile MSP

;				EXPORT Pile_PSP			; variable publique

;Stack_PSP       SPACE   Stack_Size
;Pile_PSP

; area => ALIGN implicit sur une fronti�re de 4 octets
; ici area data standard initialis�

                AREA    |.data|, DATA, READWRITE

; DCD permet l'inistalisation et la r�servation de 4 octets, il est align� sur une fronti�re de 4
;				DCD     Reset_Handler             ; Reset Handler
 

; area => ALIGN implice sur une fronti�re de 4 octets

                AREA    |.text|, CODE, READONLY

; fct appel�e par main pour passer en mode handler juste avant l'initialisation du noyau

appelSvcInitNoyau    PROC
 				EXPORT  appelSvcInitNoyau		; fonction publique

                SVC	#0							; exception logicielle
				
				; on ne reviendra plus ici mais afin de parer � tout probl�me
				; vous pouvez imaginer ici un appel � la fonction panicKernel
				
				B	.							; boucle sans fin

				ENDP

; fct appel�e lors d'une exception de type SVC (@ positionn�e dans le vecteur d'interruption de SVC)
; lors des prochaines �tapes, il faudra penser � revoir cette partie du code (migration dans entreeNoyau.s?)

SVC_Handler     PROC
 				IMPORT	initNoyau				; fct C externe
				EXPORT  SVC_Handler				; pas d'option weak (voir startup_LPC17xx.s) 
												;  => on va prendre cette �tiquette et pas
												;     d'erreur du linker pour une double d�finition

                B		initNoyau				; Branch (sans link) = goto � la fonction en C d'initialisation
												
                ENDP

				END								; Fin de l'assemblage
