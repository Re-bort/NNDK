@set s=Fwcolour
@set Count=1
@set MaxCount=5
@set Seed=200

@set m=4
@set n=7

@set Seed=20
:Loop
	som-train.exe %s%.txt %s%Cbk.txt %s%His.txt -m%m% -n%n% -C10 -D0.0001 -E2 -H%m% -L0.9 -e20 -h1 -l0.1 -y -v -w -r%seed% >> %s%Som.log
	somplex.exe %s%.txt %s%Cbk.txt %s%Tr.txt %s%Te.txt %s%Va.txt -s0.8 -t0.75 -y -v -r%seed% >> %s%.log
@REM	grnn.exe %s%Tr.txt %s%Te.txt %s%Va.txt -v -w >> %s%.log				

	@set /a Seed+=1
	@set /a Count+=1

	@if %Count% == %MaxCount% goto End 
	@goto Loop
:End


@set /p getch="Press any key to continue..."