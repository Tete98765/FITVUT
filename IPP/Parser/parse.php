<?php
    ini_set('display_errors', 'stderr');
    $inst_count = 1;
    $first_line = true;
    $symbol_arg1 = 0;
    $symbol_arg2 = 0;

    //vytvorenie xml suboru
    $xml = new DomDocument("1.0","UTF-8");
    //vypis hlavicky xml suboru
    $output = $xml->createElement('program');
    $output->setAttribute('language','IPPcode22');
    $xml->appendChild($output);
    $xml->formatOutput = true;
    
    //vypisanie napovedy
    function help() {
        fwrite(STDOUT,"Skript nacita zo standartneho vstupu zdrojovy kod v jazyku IPPcode22, skontroluje lexikalnu a syntakticku spravnost kodu a vypise na standartny vystup XML reprezentciu programu\n");
        exit(0);
    }
    
    //kontrola ci pocet argumentov pri jednotlivych instukciach bol zadany spravne
    function check_count(array $data, int $count) {
        if((count($data) - 1 ) != $count) {
            fwrite(STDERR, "Zly pocet parametrov\n");
            exit(23);
        }
    }

    //kontrola ci je var napisany spravne v tvare GF|LF|TF@...
    function check_var(string $data) {
        if (!preg_match("/^(GF|LF|TF)@[a-zA-Z_$&%<>*!?-][a-zA-Z0-9_$&%<>*!?-]*$/", $data)) {
            fwrite(STDERR, "Argument nie je validny var\n");
            exit(23);
        }
    }

    //kontrola ci je label napisany spravne 
    function check_label(string $data) {
        if (!preg_match("/^[a-zA-Z_$&%*!?-][a-zA-Z0-9_$&%*!?-]*$/", $data)) {
            fwrite(STDERR, "Argument nie je validny label\n");
            exit(23);
        }
    }

    //kontrola ci je symbol napisany spravne
    //treba rozlisovat, ci sa jedna o premennu, string, bool, nil, int a podla toho kontrolovat obsah za nou
    function check_symbol(string $data) {
        if (preg_match("/^(GF|LF|TF)@[a-zA-Z_$&<>%*!?-][\S]*/", $data)) {
            return 1;
        }
        elseif (preg_match("/^bool@(true|false)$/",$data)) {
            return 2;
        }
        elseif (preg_match("/^nil@nil$/", $data)) {
            return 3;
        }
        elseif (preg_match("/^string@([^#\\\\ \t]|(\\\\[0-9]{3}))*$/", $data)) {
            return 4;
        }
        elseif (preg_match("/^int@[+-]{0,1}[\d]+$/", $data)) {
            return 5;
        }
        else {
            fwrite(STDERR, "Argument nie je validny symbol\n");
            exit(23); 
        }
    }

    //kontrola ci je typ napisany spravne
    function check_type(string $data) {
        if (!preg_match("/^(int|string|bool|nil)$/", $data)) {
            fwrite(STDERR, "Argument nie je validny typ\n");
            exit(23);
        }
    }

    //zapis var do vystupneho xml
    //$data su data, ktore sa zapisuju, $arg_number predstavuje poradie instrukcie (ci sa jedna o arg1, arg2 atd)
    function xml_var(string $data, string $arg_number, $xml, $instruction) {
        $arg = $xml->createElement("$arg_number", $data);
        $arg->setAttribute('type','var');
        $instruction->appendChild($arg);

    }

    //zapis label do vystupneho xml
    function xml_label(string $data, string $arg_number, $xml, $instruction) {
        $arg = $xml->createElement("$arg_number", $data);
        $arg->setAttribute('type','label');
        $instruction->appendChild($arg);


    }

    //zapis symbolu do vystupneho xml
    //datovy typ sa vypise cely, z premennych vyreze cast, ktora hovori o aky datovy typ sa jedna a vypise len zvysok, ktory sa nachadza za @
    function xml_symbol(string $data, int $number, string $arg_number, $xml, $instruction) {
        if ($number  == 1) {
            $arg = $xml->createElement($arg_number, $data);
            $arg->setAttribute('type','var');
            $instruction->appendChild($arg);
        }
        elseif ($number  == 2) {
            $data = preg_replace("/^bool@/", "", $data);
            $data = trim($data);

            $arg = $xml->createElement("$arg_number", $data);
            $arg->setAttribute('type','bool');
            $instruction->appendChild($arg);
        }
        elseif ($number == 3) {
            $data = preg_replace("/^nil@/", "", $data);
            $data = trim($data); 

            $arg = $xml->createElement("$arg_number", $data);
            $arg->setAttribute('type','nil');
            $instruction->appendChild($arg);
        }
        elseif ($number == 4) {
            $data = preg_replace("/^string@/", "", $data);
            $data = trim($data);
            
            $arg = $xml->createElement("$arg_number", $data);
            $arg->setAttribute('type','string');
            $instruction->appendChild($arg);
        }
        elseif ($number == 5) {
            $data = preg_replace("/^int@/", "", $data);
            $data = trim($data);

            $arg = $xml->createElement("$arg_number", $data);
            $arg->setAttribute('type','int'); 
            $instruction->appendChild($arg);
        }

    }

    //zapis typu do vystupneho xml
    function xml_type(string $data, $xml, $instruction) {
        $arg = $xml->createElement("arg2", $data);
		$arg->setAttribute('type','type');
		$instruction->appendChild($arg);
    }

    //ak sa v stringu nachadza znak &, <, > tak ho prepise
    //funkcia sa vola pre <var> a <symb>
    function string_change(string $data) {
        $data = preg_replace('/&/', '&amp;', $data);
        $data = preg_replace('/</', '&lt;', $data);
        $data = preg_replace('/>/', '&gt;', $data);

        return $data;
    }

    //kontrola ci bol zadany nejaky argument 
    foreach($argv as $args) {
        if ($args == "--help") {
            if($argc == 2) {
               help(); 
            }
            else {
                fwrite(STDERR, "Zakazana kombinacia paramatrov");
                exit(10);
            }
            
        }
    }

    //otvorenie suboru z stdin
    $f = fopen('php://stdin', 'r');
    if (!$f) {
        fwrite(STDERR, "Chyba otvarania suboru");
        exit(11);
    }

    //prechod súborom riadok po riadku
    while($line = fgets($f)) {
        //nahradenie komantarov za whitespace
        $line = preg_replace("/#.*/", "", $line);
        
        //preskocenie prazdnych riadov
        if($line == "\n" || $line == ''){
            continue;
        }
    
        //odstranenie whitespace
        $line = trim($line);
        //rozdelenie slov podľa whitespace 
        $words_on_line = preg_split('/\s+/', $line, -1, 0); 

        //kontrola hlavičky
        if($first_line){
            $line = strtolower($line);
            if(preg_match("/^\.ippcode22$/", $line)){
                $first_line = false;
                continue;
            }
            else{
                fwrite(STDERR, "Chybna hlavicka");
                exit(21);
            }
        }

        $words_on_line[0] = strtoupper($words_on_line[0]);

        //xml zápis inštrukcie
        $instruction = $xml->createElement('instruction');
		$instruction->setAttribute('order',$inst_count);
		$instruction->setAttribute('opcode',$words_on_line[0]);
		$output->appendChild($instruction);
        $inst_count = $inst_count + 1;

        switch ($words_on_line[0]) {
            // nič
            case 'CREATEFRAME':
            case 'PUSHFRAME':
            case 'POPFRAME':
            case 'RETURN':
            case 'BREAK':
                check_count($words_on_line, 0);
                break;
                
            // <var>
            case 'DEFVAR':
            case 'POPS':
                check_count($words_on_line, 1);

                check_var($words_on_line[1]);

                $words_on_line[1] = string_change($words_on_line[1]);

                xml_var($words_on_line[1], "arg1", $xml, $instruction);
                break;
            
            // <label> 
            case 'LABEL':
            case 'JUMP':
            case 'CALL':
                check_count($words_on_line, 1);
                check_label($words_on_line[1]);

                xml_label($words_on_line[1], "arg1", $xml, $instruction);
                break;

            // <symb> 
            case 'PUSHS':
            case 'WRITE':
            case 'EXIT':
            case 'DPRINT':
                check_count($words_on_line, 1);
                $get_symb_numbr = check_symbol($words_on_line[1]);
                $words_on_line[1] = string_change($words_on_line[1]);
                xml_symbol($words_on_line[1], $get_symb_numbr, "arg1", $xml, $instruction); 
                break;
        
            // <var> <symb> 
            case 'MOVE':
            case 'NOT':
            case 'INT2CHAR':
            case 'STRLEN':
            case 'TYPE':
                check_count($words_on_line, 2);
                check_var($words_on_line[1]);
                $symbol_arg1 = check_symbol($words_on_line[2]);
                $words_on_line[1] = string_change($words_on_line[1]);
                $words_on_line[2] = string_change($words_on_line[2]);

                xml_var($words_on_line[1], "arg1", $xml, $instruction);
                xml_symbol($words_on_line[2], $symbol_arg1, "arg2", $xml, $instruction); 
                break;
                
            // <var> <type> 
            case 'READ':
                check_count($words_on_line, 2);
                check_var($words_on_line[1]);

                $words_on_line[1] = string_change($words_on_line[1]);
                check_type($words_on_line[2]);
                
                xml_var($words_on_line[1], "arg1", $xml, $instruction);
                xml_type($words_on_line[2], $xml, $instruction);
                break;

            // <label> <symb1> <symb2> 
            case 'JUMPIFEQ':
            case 'JUMPIFNEQ':
                check_count($words_on_line, 3);
                check_label($words_on_line[1]);

                $symbol_arg1 = check_symbol($words_on_line[2]);
                $symbol_arg2 = check_symbol($words_on_line[3]);

                $words_on_line[2] = string_change($words_on_line[2]);
                $words_on_line[3] = string_change($words_on_line[3]);

                xml_label($words_on_line[1], "arg1", $xml, $instruction);
                xml_symbol($words_on_line[2], $symbol_arg1, "arg2", $xml, $instruction); 
                xml_symbol($words_on_line[3], $symbol_arg2, "arg3", $xml, $instruction); 

                break;
            
            // <var> <symb1> <symb2> 
            case 'ADD':
            case 'SUB':
            case 'MUL':
            case 'IDIV':
            case 'LT':
            case 'GT':
            case 'EQ':
            case 'AND':
            case 'OR':
            case 'STRI2INT':
            case 'CONCAT':
            case 'GETCHAR':
            case 'SETCHAR':
                check_count($words_on_line, 3);
    
                check_var($words_on_line[1]);
                $symbol_arg1 = check_symbol($words_on_line[2]);
                $symbol_arg2 = check_symbol($words_on_line[3]);

                $words_on_line[1] = string_change($words_on_line[1]);
                $words_on_line[2] = string_change($words_on_line[2]);
                $words_on_line[3] = string_change($words_on_line[3]);

                xml_var($words_on_line[1], "arg1", $xml, $instruction);
                xml_symbol($words_on_line[2], $symbol_arg1, "arg2", $xml, $instruction); 
                xml_symbol($words_on_line[3], $symbol_arg2, "arg3", $xml, $instruction); 

                break;
            default:
                fwrite(STDERR, "Neznamy alebo chybny operacny kod\n");
                exit(22);
        }
    }

    //vytvorenie vysledkeho xml suboru
    echo $xml -> saveXML();
    //zatvorenie vstupneho suboru
    fclose($f);
?>