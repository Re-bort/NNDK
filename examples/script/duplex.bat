@set s=Salinity2
@set Count=1
@set MaxCount=2
@set Seed=102

:Loop
 	sample.exe %s%.txt %s%Tr.txt %s%Te.txt %s%Va.txt -s0.8 -t0.75 -v -w -r%Seed% >> %s%.log
	grnn.exe %s%Tr.txt %s%Te.txt %s%Va.txt -v -w >> %s%.log			

	@set /a Seed+=1
	@set /a Count+=1

	@if %Count% == %MaxCount% goto End 
	@goto Loop
:End

@set /p getch="Press any key to continue..."




