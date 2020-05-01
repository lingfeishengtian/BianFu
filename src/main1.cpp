#include <string>
#include <vector>
#include <iostream>

//#include "antlr4-runtime.h"
//
//#include "BFLexer.h"
//#include "BFParser.h"
//
//#include "error/BianFuErrorListener.h"
//#include "error/BianFuError.h"
//#include "scope/Scope.h"
//#include "visitors/ExecuteVisitor.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/Support/TargetSelect.h"

// Optimizations
#include "llvm/Transforms/Scalar.h"
#include "llvm/Analysis/BasicAliasAnalysis.h"

using namespace llvm;

llvm::Function* createSumFunction(Module* module) {
    /* Builds the following function:

    int sum(int a, int b) {
        int sum1 = 1 + 1;
        int sum2 = sum1 + a;
        int result = sum2 + b;
        return result;
    }
    */

    LLVMContext &context = module->getContext();
    IRBuilder<> builder(context);

    // Define function's signature
    std::vector<Type*> Integers(2, builder.getInt32Ty());
    auto *funcType = FunctionType::get(builder.getInt32Ty(), Integers, false);

    // create the function "sum" and bind it to the module with ExternalLinkage,
    // so we can retrieve it later
    auto *fooFunc = Function::Create(
            funcType, Function::ExternalLinkage, "sum", module
    );

    // Define the entry block and fill it with an appropriate code
    auto *entry = BasicBlock::Create(context, "entry", fooFunc);
    builder.SetInsertPoint(entry);

    // Add constant to itself, to visualize constant folding
    Value *constant = ConstantInt::get(builder.getInt32Ty(), 0x1);
    auto *sum1 = builder.CreateAdd(constant, constant, "sum1");

    // Retrieve arguments and proceed with further adding...
    auto args = fooFunc->arg_begin();
    Value *arg1 = &(*args);
    args = std::next(args);
    Value *arg2 = &(*args);
    auto *sum2 = builder.CreateAdd(sum1, arg1, "sum2");
    auto *result = builder.CreateAdd(sum2, arg2, "result");

    // ...and return
    builder.CreateRet(result);

    // Verify at the end
    verifyFunction(*fooFunc);
    return fooFunc;
};

int main(int argc, char* argv[]) {
    // Initilaze native target
    llvm::TargetOptions Opts;
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();

    LLVMContext context;
    auto myModule = std::make_unique<Module>("My First JIT", context);
    auto* module = myModule.get();

    std::unique_ptr<llvm::RTDyldMemoryManager> MemMgr(new llvm::SectionMemoryManager());

    // Create JIT engine
    llvm::EngineBuilder factory(std::move(myModule));
    factory.setEngineKind(llvm::EngineKind::JIT);
    factory.setTargetOptions(Opts);
    factory.setMCJITMemoryManager(std::move(MemMgr));
    auto executionEngine = std::unique_ptr<llvm::ExecutionEngine>(factory.create());
    module->setDataLayout(executionEngine->getDataLayout());

    // Create optimizations, not necessary, whole block can be ommited.
    // auto fpm = llvm::make_unique<legacy::FunctionPassManager>(module);
    // fpm->add(llvm::createBasicAAWrapperPass());
    // fpm->add(llvm::createPromoteMemoryToRegisterPass());
    // fpm->add(llvm::createInstructionCombiningPass());
    // fpm->add(llvm::createReassociatePass());
    // fpm->add(llvm::createNewGVNPass());
    // fpm->add(llvm::createCFGSimplificationPass());
    // fpm->doInitialization();

    auto* func = createSumFunction(module);  // create function
    executionEngine->finalizeObject();       // compile the module
    module->dump();                          // print the compiled code

    // Get raw pointer
    auto* raw_ptr = executionEngine->getPointerToFunction(func);
    auto* func_ptr = (int(*)(int, int))raw_ptr;

    // Execute
    int arg1 = 5;
    int arg2 = 7;
    int result = func_ptr(arg1, arg2);
    std::cout << arg1 << " + " << arg2 << " + 1 + 1 = " << result << std::endl;

    return 0;
}

//int main(int , const char **) {
//    #ifdef _WIN32
//        _setmode(_fileno(stdout), _O_U16TEXT);
//        std::wcout << L"蝙蝠Windows版没有字体颜色。抱歉。" << std::endl;
//    #endif
//
//    ANTLRInputStream input("变量 啊 = '能'\n出(啊)\n类 神{\n变量 整数的 = 2\n提(){整数的=6.8 出(整数的* 9.3 + 2 * 3)}\n}\n变量 b = 神()\nb.提()\n");
//    BFLexer lexer(&input);
//    CommonTokenStream tokens(&lexer);
//
//    BianFuLog logger = BianFuLog();
//    BianFuErrorListener bianFuErrorListener = BianFuErrorListener();
//
//    tokens.fill();
//    for (auto token : tokens.getTokens()) {
//        std::cout << token->toString() << " " << std::endl;
//    }
//
//    BFParser parser(&tokens);
//    parser.removeErrorListeners();
//    parser.addErrorListener(&bianFuErrorListener);
//
//    tree::ParseTree *tree = parser.main();
//
//    logger.log(tree->toStringTree(&parser), BianFuLog::Situation::WARNING);
//
//    Scope globalScope = Scope();
//    ExecuteVisitor executeVisitor = ExecuteVisitor(&globalScope);
//    try{
//        executeVisitor.visitMain(dynamic_cast<BFParser::MainContext *>(tree));
//    }catch (BianFuError &error){
//        error.logError();
//        logger.log("蝙蝠程序失败", BianFuLog::Situation::ERROR);
//    }
//
//    return 0;
//}