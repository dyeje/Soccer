del *.cpp 
del *.h 

xcopy ..\BlazersTeam\*.* /y

ren Blazers*.* Prior__*.*

call search_replace.vbs Blazers Prior__ Prior__Team.h
call search_replace.vbs Blazers Prior__ Prior__Team.cpp
call search_replace.vbs BlazersTeam Prior__Team Prior__Team.cpp

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

call search_replace.vbs Blazers Prior__ Prior__FieldPlayerStates.h
call search_replace.vbs Blazers Prior__ Prior__FieldPlayerStates.cpp
call search_replace.vbs BlazersFieldPlayerStates Prior__FieldPlayerStates Prior__FieldPlayerStates.cpp
call search_replace.vbs BlazersTeam Prior__Team Prior__FieldPlayerStates.cpp

call search_replace.vbs Blazers Prior__ Prior__GoalKeeperStates.h
call search_replace.vbs Blazers Prior__ Prior__GoalKeeperStates.cpp
call search_replace.vbs BlazersGoalKeeper Prior__GaolKeeper Prior__GoalKeeperStates.cpp

call search_replace.vbs Blazers Prior__ Prior__SupportSpotCalculator.h
call search_replace.vbs Blazers Prior__ Prior__SupportSpotCalculator.cpp


