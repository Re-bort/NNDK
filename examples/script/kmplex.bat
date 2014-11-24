@set s=hillval
@set Count=1
@set MaxCount=2
@set Seed=200

@set k=30


@set Seed=20
@REM :Loop
	km-train.exe %s%.txt %s%Cbk.txt %s%His.txt -k%k% -l1.0 -e100 -v -w -r%seed% >> %s%KM.log
	
@REM KMplex.exe %s%.txt %s%Cbk.txt %s%Tr.txt %s%Te.txt %s%Va.txt -T0.6 -t0.2 -y -v -r%seed% >> %s%.log
@REM	grnn.exe %s%Tr.txt %s%Te.txt %s%Va.txt -v -w >> %s%.log				

@REM	@set /a Seed+=1
@REM	@set /a Count+=1

@REM	@if %Count% == %MaxCount% goto End 
@REM	@goto Loop
@REM :End


@set /p getch="Press any key to continue..."