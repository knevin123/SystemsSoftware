objects=daemon.o backup.o update.o updateAudit.o locking.o
headers=update.h backup.h updateAudit.h locking.h

daemon: $(objects)
	gcc -o daemon $(objects) -lm -lrt

daemon.o: daemon.c $(headers)
	gcc -c daemon.c

backup.o :backup.c
	gcc -c backup.c

update.o :update.c
	gcc -c update.c
    
updateAudit.o :updateAudit.c
	gcc -c updateAudit.c

locking.o :locking.c
	gcc -c locking.c

backup:
	gcc backupOndemand.c -o backup 

transfer:
	gcc transferOnDemand.c -o transfer 

queue:
	gcc queue.c -o queue 
    
clean:
	rm daemon $(objects) 
    