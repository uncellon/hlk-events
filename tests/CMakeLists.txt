add_executable(FunctionDelegateCallTest functiondelegatecall.cpp)
target_link_libraries(FunctionDelegateCallTest ${PROJECT_NAME})

add_executable(MethodDelegateCallTest methoddelegatecall.cpp)
target_link_libraries(MethodDelegateCallTest ${PROJECT_NAME})

add_executable(LambdaDelegateCallTest lambdadelegatecall.cpp)
target_link_libraries(LambdaDelegateCallTest ${PROJECT_NAME})

add_executable(RemovedMethodCallTest removedmethodcall.cpp)
target_link_libraries(RemovedMethodCallTest ${PROJECT_NAME})

add_executable(RemovedMethodCallV2Test removedmethodcallv2.cpp)
target_link_libraries(RemovedMethodCallV2Test ${PROJECT_NAME})

add_executable(RemovedLambdaCallTest removedlambdacall.cpp)
target_link_libraries(RemovedLambdaCallTest ${PROJECT_NAME})