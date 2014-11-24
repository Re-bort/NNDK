@set s=Salinity10T

findstr Trg %s%.log > %s%Tr.log
findstr Tst %s%.log > %s%Te.log
findstr Vln %s%.log > %s%Va.log
findstr h: %s%.log > %s%Bw.log


@set /p getch="Press any key to continue..."

