@set s=Salinity10T

@set Count=1
@set MaxCount=2
@set Seed=101

@set m=16
@set n=10

som-train.exe %s%.txt %s%Cbk.txt %s%His.txt -m%m% -n%n% -C10 -D0.0001 -E2 -H%m% -L0.9 -e20 -h1 -l0.1 -y -v -w -r%seed% >> %s%Som.log

@set Seed=116
:Loop
	som-sample.exe %s%.txt %s%Cbk.txt %s%Tr.txt %s%Te.txt %s%Va.txt -p -s0.5 -t0.75 -y -v -r%seed% >> %s%.log
	
 	som-query.exe %s%Tr.txt %s%Cbk.txt -v -y -m%m% -n%n% >> %s%Som.log
	som-query.exe %s%Te.txt %s%Cbk.txt -v -y -m%m% -n%n% >> %s%Som.log
	som-query.exe %s%Va.txt %s%Cbk.txt -v -y -m%m% -n%n% >> %s%Som.log

	grnn.exe %s%Tr.txt %s%Te.txt %s%Va.txt -v -w >> %s%.log				

	@set /a Seed+=1
	@set /a Count+=1

	@if %Count% == %MaxCount% goto End 
	@goto Loop
:End


@set /p getch="Press any key to continue..."