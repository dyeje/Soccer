del *.cpp 
del *.h 

xcopy ..\BlazersTeam\*.* /y

ren Blazers*.* Prior__*.*

call search_replace.vbs Blazers Prior__ Prior__Team.h
call search_replace.vbs Blazers Prior__ Prior__Team.cpp

call search_replace.vbs Blazers Prior__ Prior__TeamMaker.h
call search_replace.vbs Blazers Prior__ Prior__TeamMaker.cpp

call search_replace.vbs Blazers Prior__ Prior__States.h
call search_replace.vbs Blazers Prior__ Prior__States.cpp

call search_replace.vbs Blazers Prior__ Prior__GoalKeeper.h
call search_replace.vbs Blazers Prior__ Prior__GoalKeeper.cpp

call search_replace.vbs Blazers Prior__ Prior__FieldPlayer.h
call search_replace.vbs Blazers Prior__ Prior__FieldPlayer.cpp

call search_replace.vbs Blazers Prior__ GoalKeeperStates.h
call search_replace.vbs Blazers Prior__ GoalKeeperStates.cpp

call search_replace.vbs Blazers Prior__ FieldPlayerStates.h
call search_replace.vbs Blazers Prior__ FieldPlayerStates.cpp



