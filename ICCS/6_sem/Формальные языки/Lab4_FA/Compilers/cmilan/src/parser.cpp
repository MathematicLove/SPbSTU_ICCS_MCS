#include "parser.h"
#include <sstream>

//Выполняем синтаксический разбор блока program. Если во время разбора не обнаруживаем 
//никаких ошибок, то выводим последовательность команд стек-машины
void Parser::parse()
{
	program(); 
	if(!error_) {
		codegen_->flush();
	}
}

void Parser::program()
{
	mustBe(T_BEGIN);
	statementList();
	mustBe(T_END);
	codegen_->emit(STOP);
}

void Parser::statementList()
{
	//	  Если список операторов пуст, очередной лексемой будет одна из возможных "закрывающих скобок": END, OD, ELSE, FI.
	//	  В этом случае результатом разбора будет пустой блок (его список операторов равен null).
	//	  Если очередная лексема не входит в этот список, то ее мы считаем началом оператора и вызываем метод statement. 
	//    Признаком последнего оператора является отсутствие после оператора точки с запятой.
	if(see(T_END) || see(T_OD) || see(T_ELSE) || see(T_FI)) {
		return;
	}
	else {
		bool more = true;
		while(more) {
			statement();
			more = match(T_SEMICOLON);
		}
	}
}

void Parser::statement()
{
	// Если встречаем переменную, то запоминаем ее адрес или добавляем новую если не встретили. 
	// Следующей лексемой должно быть присваивание. Затем идет блок expression, который возвращает значение на вершину стека.
	// Записываем это значение по адресу нашей переменной
	if(see(T_IDENTIFIER)) {
		int varAddress = findOrAddVariable(scanner_->getStringValue());
		next();
		mustBe(T_ASSIGN);
		expression();
		codegen_->emit(STORE, varAddress);
	}
	// Если встретили IF, то затем должно следовать условие. На вершине стека лежит 1 или 0 в зависимости от выполнения условия.
	// Затем зарезервируем место для условного перехода JUMP_NO к блоку ELSE (переход в случае ложного условия). Адрес перехода
	// станет известным только после того, как будет сгенерирован код для блока THEN.
	else if(match(T_IF)) {
		relation();
		
		int jumpNoAddress = codegen_->reserve();

		mustBe(T_THEN);
		statementList();
		if(match(T_ELSE)) {
		//Если есть блок ELSE, то чтобы не выполнять его в случае выполнения THEN, 
		//зарезервируем место для команды JUMP в конец этого блока
			int jumpAddress = codegen_->reserve();
		//Заполним зарезервированное место после проверки условия инструкцией перехода в начало блока ELSE.
			codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
			statementList();
		//Заполним второй адрес инструкцией перехода в конец условного блока ELSE.
			codegen_->emitAt(jumpAddress, JUMP, codegen_->getCurrentAddress());
		}
		else {
		//Если блок ELSE отсутствует, то в зарезервированный адрес после проверки условия будет записана
		//инструкция условного перехода в конец оператора IF...THEN
			codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
		}

		mustBe(T_FI);
	}

	else if(match(T_WHILE)) {
		//запоминаем адрес начала проверки условия.
		int conditionAddress = codegen_->getCurrentAddress();
		relation();
		//резервируем место под инструкцию условного перехода для выхода из цикла.
		int jumpNoAddress = codegen_->reserve();
		mustBe(T_DO);
		statementList();
		mustBe(T_OD);
		//переходим по адресу проверки условия
		codegen_->emit(JUMP, conditionAddress);
		//заполняем зарезервированный адрес инструкцией условного перехода на следующий за циклом оператор.
		codegen_->emitAt(jumpNoAddress, JUMP_NO, codegen_->getCurrentAddress());
	}
    
    // 4) for <ident> := <expr>,<expr>[,<expr>] do <statementList> od
        else if (match(T_FOR)) {
            // 4.1) Имя переменной цикла
            if (!see(T_IDENTIFIER)) {
                reportError("identifier expected after 'for'");
                return;
            }
            string loopVar = scanner_->getStringValue();
            // 4.2) Проверяем вложенность
            if (find(loopVars_.begin(), loopVars_.end(), loopVar) != loopVars_.end()) {
                reportError("nested loop variable '" + loopVar + "' redeclared");
            }
            loopVars_.push_back(loopVar);
            int vAddr = findOrAddVariable(loopVar);
            next(); // съесть имя

            // 4.3) Начальное значение
            mustBe(T_ASSIGN);
            expression();
            codegen_->emit(STORE, vAddr);

            // 4.4) Конечное значение
            mustBe(T_COMMA);
            int limAddr = findOrAddVariable("__lim");
            expression();
            codegen_->emit(STORE, limAddr);

            // 4.5) Шаг (возможно отрицательный)
            int stepAddr = findOrAddVariable("__step");
            if (match(T_COMMA)) {
                expression();
            } else {
                codegen_->emit(PUSH, 1);
            }
            codegen_->emit(STORE, stepAddr);
            
            // 4.6) Условие и тело цикла: (i - lim) * step <= 0
            mustBe(T_DO);
            int condAddr = codegen_->getCurrentAddress();

            // загрузить i и lim
            codegen_->emit(LOAD, vAddr);       // стек: [ ... , i ]
            codegen_->emit(LOAD, limAddr);     // стек: [ ... , i, lim ]
            codegen_->emit(SUB);               // стек: [ ... , (i - lim) ]

            // загрузить step и умножить
            codegen_->emit(LOAD, stepAddr);    // стек: [ ... , (i - lim), step ]
            codegen_->emit(MULT);              // стек: [ ... , (i - lim)*step ]

            // сравнить с нулём: (i-lim)*step <= 0 ?
            codegen_->emit(PUSH, 0);           // стек: [ ... , (i-lim)*step, 0 ]
            codegen_->emit(COMPARE, 4);        // <=
            // зарезервировать переход при ложно́м условии
            int jmpNo = codegen_->reserve();

            statementList();
            mustBe(T_OD);
            // 4.7) Инкремент и выход
            codegen_->emit(LOAD, vAddr);
            codegen_->emit(LOAD, stepAddr);
            codegen_->emit(ADD);
            codegen_->emit(STORE, vAddr);
            codegen_->emit(JUMP, condAddr);
            codegen_->emitAt(jmpNo, JUMP_NO, codegen_->getCurrentAddress());

            // 4.8) Удаляем имя из стека вложенных циклов
            loopVars_.pop_back();
        }
        // 5) write(expr)
        else if (match(T_WRITE)) {
            mustBe(T_LPAREN);
            expression();
            mustBe(T_RPAREN);
            codegen_->emit(PRINT);
        }
        // 6) Ошибка
        else {
            reportError("statement expected.");
        }
    }

    // <expression> ::= <term> { (+|-) <term> }
    void Parser::expression()
    {
        term();
        while (see(T_ADDOP)) {
            Arithmetic op = scanner_->getArithmeticValue();
            next();
            term();
            codegen_->emit(op == A_PLUS ? ADD : SUB);
        }
    }

void Parser::term()
{
	 /*  
		 Терм описывается следующими правилами: <expression> -> <factor> | <factor> + <factor> | <factor> - <factor>
         При разборе сначала смотрим первый множитель, затем анализируем очередной символ. Если это '*' или '/', 
		 удаляем его из потока и разбираем очередное слагаемое (вычитаемое). Повторяем проверку и разбор очередного 
		 множителя, пока не встретим за ним символ, отличный от '*' и '/' 
	*/
	factor();
	while(see(T_MULOP)) {
		Arithmetic op = scanner_->getArithmeticValue();
		next();
		factor();

		if(op == A_MULTIPLY) {
			codegen_->emit(MULT);
		}
		else {
			codegen_->emit(DIV);
		}
	}
}

void Parser::factor()
{
	/*
		Множитель описывается следующими правилами:
		<factor> -> number | identifier | -<factor> | (<expression>) | READ
	*/
	if(see(T_NUMBER)) {
		int value = scanner_->getIntValue();
		next();
		codegen_->emit(PUSH, value);
		//Если встретили число, то преобразуем его в целое и записываем на вершину стека
	}
	else if(see(T_IDENTIFIER)) {
		int varAddress = findOrAddVariable(scanner_->getStringValue());
		next();
		codegen_->emit(LOAD, varAddress);
		//Если встретили переменную, то выгружаем значение, лежащее по ее адресу, на вершину стека 
	}
	else if(see(T_ADDOP) && scanner_->getArithmeticValue() == A_MINUS) {
		next();
		factor();
		codegen_->emit(INVERT);
		//Если встретили знак "-", и за ним <factor> то инвертируем значение, лежащее на вершине стека
	}
	else if(match(T_LPAREN)) {
		expression();
		mustBe(T_RPAREN);
		//Если встретили открывающую скобку, тогда следом может идти любое арифметическое выражение и обязательно
		//закрывающая скобка.
	}
	else if(match(T_READ)) {
		codegen_->emit(INPUT);
		//Если встретили зарезервированное слово READ, то записываем на вершину стека идет запись со стандартного ввода
	}
	else {
		reportError("expression expected.");
	}
}

void Parser::relation()
{
	//Условие сравнивает два выражения по какому-либо из знаков. Каждый знак имеет свой номер. В зависимости от 
	//результата сравнения на вершине стека окажется 0 или 1.
	expression();
	if(see(T_CMP)) {
		Cmp cmp = scanner_->getCmpValue();
		next();
		expression();
		switch(cmp) {
			//для знака "=" - номер 0
			case C_EQ:
				codegen_->emit(COMPARE, 0);
				break;
			//для знака "!=" - номер 1
			case C_NE:
				codegen_->emit(COMPARE, 1);
				break;
			//для знака "<" - номер 2
			case C_LT:
				codegen_->emit(COMPARE, 2);
				break;
			//для знака ">" - номер 3
			case C_GT:
				codegen_->emit(COMPARE, 3);
				break;
			//для знака "<=" - номер 4
			case C_LE:
				codegen_->emit(COMPARE, 4);
				break;
			//для знака ">=" - номер 5
			case C_GE:
				codegen_->emit(COMPARE, 5);
				break;
		};
	}
	else {
		reportError("comparison operator expected.");
	}
}

int Parser::findOrAddVariable(const string& var)
{
	VarTable::iterator it = variables_.find(var);
	if(it == variables_.end()) {
		variables_[var] = lastVar_;
		return lastVar_++;
	}
	else {
		return it->second;
	}
}

void Parser::mustBe(Token t)
{
	if(!match(t)) {
		error_ = true;

		// Подготовим сообщение об ошибке
		std::ostringstream msg;
		msg << tokenToString(scanner_->token()) << " found while " << tokenToString(t) << " expected.";
		reportError(msg.str());

		// Попытка восстановления после ошибки.
		recover(t);
	}
}

void Parser::recover(Token t)
{
	while(!see(t) && !see(T_EOF)) {
		next();
	}

	if(see(t)) {
		next();
	}
}
