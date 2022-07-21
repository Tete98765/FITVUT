import re
import xml.etree.ElementTree as ET
import argparse
import sys

class labels:
    label_list = list()
    label_index = list()
    call_list = list()
    call_index = list()

class frames:
    global_frame = list()
    global_values = list()
    global_types = list()
    
    local_frame = list()
    local_values = list()
    local_types = list()

    tmp_frame = list()
    tmp_values = list()
    tmp_types = list()

    stack = list()
    stack_types = list()

class Argument:
  # class construtor
  def __init__(self, argType, value):
    self.type = argType
    self.value = value

class Instruction:
  # class constructor
  def __init__(self, name, number):
    self.name = name
    self.number = number
    self.args = []
  
  def addArgument(self, argType, value):
    self.args.append(Argument(argType, value))

def save_var(var, result, d_type):
    var = var.value.split("@")
    if var[0] == "GF":
        if var[1] in frames.global_frame:    
            where = frames.global_frame.index(var[1])
            frames.global_values[where] = result
            frames.global_types[where] = d_type
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if var[0] == "LF":
        if var[1] in frames.local_frame:
            where = frames.local_frame.index(var[1])
            frames.local_values[where] = result
            frames.local_types[where] = d_type
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if var[0] == "TF": 
        if var[1] in frames.tmp_frame:
            where = frames.tmp_frame.index(var[1])
            frames.tmp__value[where] = result
            frames.tmp_types[where] = d_type
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)

def eq(symb1, symb2):
    if symb1.type == "int" and symb2.type == "int":
        if int(symb1.value) == int(symb2.value):
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if int(tmp_var) == int(symb2.value):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if int(symb1.value) == int(tmp_var):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "bool" and symb2.type == "bool":
        if (symb2.value == "false" and symb1.value == "false") or (symb2.value == "true" and symb1.value == "true"):
            result = "true"
        else:
            result = "false"
    elif symb1.type == "var" and symb2.type == "bool":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if (tmp_var == "true" and symb2.value == "true") or (tmp_var == "false" and symb2.value == "false"):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "bool" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if (symb1.value == "true" and tmp_var == "true") or (symb1.value == "false" and tmp_var == "false"):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "string" and symb2.type == "string":
        if symb1.value == symb2.value:
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "var" and symb2.type == "string":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if tmp_var == symb2.value:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "string" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if symb1.value == tmp_var:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 

    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        tmp1_type, tmp_var1 = var_type(tmp_var1)
        tmp2_type, tmp_var2 = var_type(tmp_var2)

        if tmp1_type == tmp2_type:
            if tmp_var1 == tmp_var2:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "nil" and symb2.type == "nil":
        if symb1.value == symb2.value:
            result = "true"
        else:
            result = "false"
    elif symb1.type == "nil" and symb2.type == "var":
        var = symb2.type.split("@")
        if var[0] == "GF":
            if var[1] in frames.global_frame:    
                where = frames.global_frame.index(var[1])
                if frames.global_types[where] == "nil":
                    result == "true"
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "LF":
            if var[1] in frames.local_frame:
                where = frames.local_frame.index(var[1])
                if frames.local_types[where] == "nil":
                    result == "true"
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "TF": 
            if var[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(var[1])
                if frames.tmp_types[where] == "nil":
                    result == "true"
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
    elif symb1.type == "var" and symb2.type == "nil":
        var = symb1.type.split("@")
        if var[0] == "GF":
            if var[1] in frames.global_frame:    
                where = frames.global_frame.index(var[1])
                if frames.global_types[where] == "nil":
                    result == "true"
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "LF":
            if var[1] in frames.local_frame:
                where = frames.local_frame.index(var[1])
                if frames.local_types[where] == "nil":
                    result == "true"
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "TF": 
            if var[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(var[1])
                if frames.tmp_types[where] == "nil":
                    result == "true"
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    return result

def check_move(instruction):
    var = instruction.args[0] #kam ukladam
    symb = instruction.args[1] #co ukladam   

    value = ''
    #kontrola symbol
    d_type = symb.type
    if d_type in ("int", "string", "nil", "bool"):
        value = symb.value
    else:
        symb = symb.value.split("@")
        if symb[0] == "GF":
            if not frames.global_values:
                print("Uninitialized variable", file=sys.stderr)
                exit(56)
            elif symb[1] in frames.global_frame:    
                where = frames.global_frame.index(symb[1])
                value = frames.global_values[where]
                d_type = frames.global_types[where]
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if symb[0] == "LF":
            if not frames.local_values:
                print("Uninitialized variable", file=sys.stderr)
                exit(56)
            elif symb[1] in frames.local_frame:
                where = frames.local_frame.index(symb[1])
                value = frames.local_values[where]
                d_type = frames.local_types[where]
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if symb[0] == "TF": 
            if not frames.tmp_values:
                print("Uninitialized variable", file=sys.stderr)
                exit(56)
            elif symb[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(symb[1])
                value = frames.tmp_values[where]
                d_type = frames.tmp_types[where]
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
    #zapis do var
    save_var(var, value, d_type)

def check_createframe():
    frames.tmp_frame.clear()
    frames.tmp_values.clear()
    frames.tmp_typess.clear()
    frames.tmp_frame.insert(0, '')
    frames.tmp_values.insert(0, '')
    frames.tmp_types.insert(0, '')

def check_pushframe():
    if not frames.tmp_frame:
        print("Temporary frame doesn't exists",file=sys.stderr)
        exit(54) 
    else:
        frames.local_frame.append(frames.tmp_frame) 
        frames.local_values.append(frames.tmp_values) 
        frames.local_typess.append(frames.tmp_types) 
        frames.tmp_frame.clear()
        frames.tmp_values.clear()
        frames.tmp_types.clear()

def check_popframe():
    if not frames.local_frame:
        print("Local frame is empty",file=sys.stderr)
        exit(55) 
    else:
        frames.tmp_frame = frames.local_frame.pop() 
        frames.tmp_values = frames.local_values.pop()
        frames.tmp_types = frames.local_types.pop()

def check_defvar(var):
    var = var.value.split("@")

    if var[0] == "GF":
        if var[1] in frames.global_frame:
            print("Variable already exists",file=sys.stderr)
            exit(52)
        else:
            frames.global_frame.append(var[1])
            frames.global_values.append('')
            frames.global_types.append('')
    if var[0] == "LF":
        if var[1] in frames.local_frame:
            print("Variable already exists",file=sys.stderr)
            exit(52)
        else:
            frames.local_frame.append(var[1])
            frames.local_values.append('')
            frames.local_types.append('')
    if var[0] == "TF": 
        if var[1] in frames.tmp_frame:
            print("Variable already exists",file=sys.stderr)
            exit(52)
        else:
            frames.tmp_frame.append(var[1])
            frames.tmp_values.append('') 
            frames.tmp_types.append('')   

def check_call(label):
    check_jump(label)

def check_return():
    global pos
    if len(labels.call_list) == 0:
        print("No call where I can return",file=sys.stderr)
        exit(56)
    else:
        position = labels.call_index.pop()
        pos = position - 1

def check_pushs(symb):
    d_type = symb.type
    if d_type in ("int", "string", "nil", "bool"):
        frames.stack.append(symb.value)
        frames.stack_types.append(symb.type)

    symb = symb.value.split("@")

    if symb[0] == "GF":
        if symb[1] in frames.global_frame:
            where = frames.global_frame.index(symb[1])
            frames.stack.append(frames.global_values[where])
            frames.stack_types.append(frames.global_types[where])
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    elif symb[0] == "LF":
        if symb[1] in frames.local_frame:
            where = frames.local_frame.index(symb[1])
            frames.stack.append(frames.local_values[where])
            frames.stack_types.append(frames.local_types[where])
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    elif symb[0] == "TF":
        if symb[1] in frames.tmp_frame:
            where = frames.tmp_frame.index(symb[1])
            frames.stack.append(frames.tmp_values[where])
            frames.stack_types.append(frames.tmp_types[where])
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)

def check_pops(var):
    var = var.value.split("@")
    #value = frames.stack[0]
    if frames.stack:
        value = frames.stack.pop()
        d_type = frames.stack_types.pop()
    
        if var[0] == "GF":
            if var[1] in frames.global_frame:
                where = frames.global_frame.index(var[1])
                frames.global_values[where] = value
                frames.global_types[where] = d_type
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "LF":
            if var[1] in frames.local_frame:
                where = frames.local_frame.index(var[1])
                frames.local_values[where] = value
                frames.local_types[where] = d_type
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "TF": 
            if var[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(var[1])
                frames.tmp_values[where] = value
                frames.tmp_types[where] = d_type
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54) 
        
    else:
        print("Stack is empty",file=sys.stderr)
        exit(56)

def var_for_result(tmp_var):
    if tmp_var[0] == "GF":
        if tmp_var[1] in frames.global_frame:
            where = frames.global_frame.index(tmp_var[1])
            if frames.global_types[where] != "int":
                print("Invalid type",file=sys.stderr)
                exit(53)
            else:
                value = frames.global_values[where]
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if tmp_var[0] == "LF":
        if var[1] in frames.local_frame:
            where = frames.local_frame.index(tmp_var[1])
            if frames.local_types[where] != "int":
                print("Invalid type",file=sys.stderr)
                exit(53)
            else:
                value = frames.local_values[where]
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if tmp_var[0] == "TF": 
        if tmp_var[1] in frames.tmp_frame:
            where = frames.tmp_frame.index(tmp_var[1])
            if frames.tmp_types[where] != "int":
                print("Invalid type",file=sys.stderr)
                exit(53)
            else:
                value = frames.tmp_values[where]
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54) 

    value = int(value)
    return value

def check_add(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "int" and symb2.type == "int":
        result = int(symb1.value) + int(symb2.value)

    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        value = var_for_result(tmp_var)
        result = value + int(symb1.value)

    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        value = var_for_result(tmp_var)
        result = value + int(symb2.value)

    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        value1 = var_for_result(tmp_var1)
        value2 = var_for_result(tmp_var2)

        result = value1 + value2
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    #zapis do var
    save_var(var, result, "int")

def check_sub(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "int" and symb2.type == "int":
        result = int(symb1.value) - int(symb2.value)   
    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        value = var_for_result(tmp_var)
        result = int(symb1.value) - value
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        value = var_for_result(tmp_var)
        result = value - int(symb2.value)
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        value1 = var_for_result(tmp_var1)
        value2 = var_for_result(tmp_var2)

        result = value1 - value2
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    #zapis do var
    save_var(var, result, "int")

def check_mul(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "int" and symb2.type == "int":
        result = int(symb1.value) * int(symb2.value)   
    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        value = var_for_result(tmp_var)
        result = value * int(symb1.value)
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        value = var_for_result(tmp_var)
        result = value * int(symb2.value)
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        value1 = var_for_result(tmp_var1)
        value2 = var_for_result(tmp_var2)

        result = value1 * value2
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    #zapis do var
    save_var(var, result, "int")

def check_idiv(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "int" and symb2.type == "int":
        if symb2.type == "0":
            print("Division by zero",file=sys.stderr)
            exit(57)
        else:
            result = int(symb1.value) / int(symb2.value)   
    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        value = var_for_result(tmp_var)

        if value == 0:
            print("Division by zero",file=sys.stderr)
            exit(57)
        else:
            result = int(symb1.value) / value
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        value = var_for_result(tmp_var)

        if symb2.type == "0":
            print("Division by zero",file=sys.stderr)
            exit(57)
        else:
            result = value / int(symb2.value)
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        value1 = var_for_result(tmp_var1)
        value2 = var_for_result(tmp_var2)

        if value2 == 0:
            print("Division by zero",file=sys.stderr)
            exit(57)
        else:
            result = value1 / value2
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    result = int(result)

    #zapis do var
    save_var(var, result, "int")

def var_type(var):
    if var[0] == "GF":
        if var[1] in frames.global_frame:    
            where = frames.global_frame.index(var[1])
            value = frames.global_values[where]
            d_type = frames.global_types[where]
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if var[0] == "LF":
        if var[1] in frames.local_frame:
            where = frames.local_frame.index(var[1])
            value = frames.local_values[where] = result
            d_type = frames.local_types[where]
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if var[0] == "TF": 
        if var[1] in frames.tmp_frame:
            where = frames.tmp_frame.index(var[1])
            value = frames.tmp__value[where] = result
            d_type = frames.tmp_types[where]
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)

    return d_type, value  

def check_lt(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "int" and symb2.type == "int":
        if int(symb1.value) < int(symb2.value):
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if int(tmp_var) < int(symb2.value):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if int(symb1.value) < int(tmp_var):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "bool" and symb2.type == "bool":
        if symb2.value == "false" and symb1.value == "true":
            result = "true"
        else:
            result = "false"
    elif symb1.type == "var" and symb2.type == "bool":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if tmp_var == "true" and symb2.value == "false":
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "bool" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if symb1.value == "true" and tmp_var == "false":
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "string" and symb2.type == "string":
        if symb1.value > symb2.value:
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "string" and symb2.type == "string":
        if symb1.value > symb2.value:
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "var" and symb2.type == "string":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if tmp_var > symb2.value:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "string" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if symb1.value > tmp_var:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 

    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        tmp1_type, tmp_var1 = var_type(tmp_var1)
        tmp2_type, tmp_var2 = var_type(tmp_var2)

        if tmp1_type == tmp2_type:
            if tmp_var1 > tmp_var2:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    #zapis do var
    save_var(var, result, "bool")

def check_gt(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "int" and symb2.type == "int":
        if symb1.value > symb2.value:
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if tmp_var > symb2.value:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "int" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if symb1.value > tmp_var:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "bool" and symb2.type == "bool":
        if symb2.value == "false" and symb1.value == "true":
            result = "true"
        else:
            result = "false"
    elif symb1.type == "var" and symb2.type == "bool":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if tmp_var == "true" and symb2.value == "false":
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "bool" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if symb1.value == "true" and tmp_var == "false":
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)

    elif symb1.type == "string" and symb2.type == "string":
        if symb1.value > symb2.value:
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "string" and symb2.type == "string":
        if symb1.value > symb2.value:
            result = "true"
        else:
            result = "false"  
    elif symb1.type == "var" and symb2.type == "string":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb2.type:
            if tmp_var > symb2.value:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    elif symb1.type == "string" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == symb1.type:
            if symb1.value > tmp_var:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 

    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        tmp1_type, tmp_var1 = var_type(tmp_var1)
        tmp2_type, tmp_var2 = var_type(tmp_var2)

        if tmp1_type == tmp2_type:
            if tmp_var1 > tmp_var2:
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    #zapis do var
    save_var(var, result, "bool")

def check_eq(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    result = eq(symb1, symb2)

    #zapis do var
    save_var(var, result, "bool")

def check_and(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]
 
    if symb1.type == "bool" and symb2.type == "bool":
        if symb1.value == "true" and symb2.value == "true":
            result = "true"
        else:
            result = "false" 
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        tmp1_type, tmp_var1 = var_type(tmp_var1)
        tmp2_type, tmp_var2 = var_type(tmp_var2)

        if tmp1_type == "bool" and tmp2_type == "bool":
            if tmp_var1 == "true" and tmp_var2 == "true":
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    save_var(var, result, "bool")

def check_or(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]
 
    if symb1.type == "bool" and symb2.type == "bool":
        if (symb1.value == "true" and symb2.value == "true") or (symb1.value == "true" and symb2.value == "false") or (symb1.value == "false" and symb2.value == "true"):
            result = "true"
        else:
            result = "false" 
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        tmp1_type, tmp_var1 = var_type(tmp_var1)
        tmp2_type, tmp_var2 = var_type(tmp_var2)

        if tmp1_type == "bool" and tmp2_type == "bool":
            if (tmp_var1 == "true" and tmp_var2 == "true") or (tmp_var1 == "true" and tmp_var2 == "false") or (tmp_var1 == "false" and tmp_var2 == "true"):
                result = "true"
            else:
                result = "false"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    save_var(var, result, "bool")

def check_not(instruction):
    var = instruction.args[0] #kam nacitam
    symb = instruction.args[1] #aky typ nacitam 
 
    if symb.type == "bool":
        if symb.value == "false":
            result = "true"
        else:
            result = "false" 
    elif symb.type == "var":
        tmp_var = symb.value.split("@")

        tmp_type, tmp_var = var_type(tmp_var)

        if tmp_type == "bool":
            if tmp_var == "true":
                result = "false"
            else:
                result = "true"
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    save_var(var, result, "bool")    

def check_int2char(instruction):
    var = instruction.args[0] #kam nacitam
    symb = instruction.args[1] #aky typ nacitam   

    if symb.type == "int":
        if int(symb.value) < 1 or int(symb.value) > 1114111:
            print("Value is out of unicode range",file=sys.stderr)
            exit(58)
        else:
            result = chr(int(symb.value))
    elif symb.type == "var":
        tmp_var = symb.value.split("@")

        tmp_type, tmp_var = var_type(tmp_var)
        tmp_var = int(tmp_var)

        if tmp_type == "int":
            if tmp_var < 1 or tmp_var > 1114111:
                print("Value is out of unicode range",file=sys.stderr)
                exit(58)
            else:
                result = chr(tmp_var)
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)
    
    save_var(var, result, "string")  

def check_stri2int(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam   
    symb2 = instruction.args[2]

    if symb1.type == "string" and symb2.type == "int":
        str_len = len(symb1.value)
        if str_len < int(symb2.value):
            print("Invalid index",file=sys.stderr)
            exit(58) 
        else:
            letter = symb1.value[int(symb2.value)]
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)
        if tmp_type == "string":
            str_len = len(tmp_var)
            if str_len < int(symb2.value):
                print("Invalid index",file=sys.stderr)
                exit(58) 
            else:
                letter = tmp_var[int(symb2.value)]
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 

    elif symb1.type == "string" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)
        if tmp_type == "int":
            str_len = len(symb1.value)
            if str_len < int(tmp_var):
                print("Invalid index",file=sys.stderr)
                exit(58) 
            else:
                letter = symb1.value[int(tmp_var)]
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 

    elif symb1.type == "var" and symb2.type == "var":
        tmp1_var = symb1.value.split("@")
        tmp2_var = symb2.value.split("@")
        tmp1_type, tmp1_var = var_type(tmp1_var)
        tmp2_type, tmp2_var = var_type(tmp2_var)

        if tmp1_type == "string" and tmp2_type == "int":
            str_len = len(tmp1_var)
            if str_len < int(tmp2_var):
                print("Invalid index",file=sys.stderr)
                exit(58) 
            else:
                letter = tmp1_var[int(tmp2_var)]
        else:
            print("Invalid type",file=sys.stderr)
            exit(53) 
 
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)
    
    letter = ord(letter)
    save_var(var, letter, "int") 

def check_read(instruction):
    var = instruction.args[0] #kam nacitam
    d_type = instruction.args[1] #aky typ nacitam   

    if d_type == "int":
        value = input()
        value = int(value)
    elif d_type == "bool":
        value = input()
        if input == "true":
            value == "true"
        else:
            value == "false"
    elif d_type == "string":
        value = input()
    else:
        value = "nil"
    #todo error???
    #zapis do var
    save_var(var, value, d_type)

def check_write(symb):
    d_type = symb.type

    if d_type == "bool":
        if symb.value == "true":
            print("true", end='')
        else:
            print("false", end='')

    if d_type == "nil":
        print("", end='')

    if d_type in ("int", "string"):
        value = symb.value
        print(value, end='')

    if d_type == "var":
        var = symb.value.split("@")

        if var[0] == "GF":
            if var[1] in frames.global_frame:
                where = frames.global_frame.index(var[1])
                value = frames.global_values[where]
                print(value, end='')
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "LF":
            if var[1] in frames.local_frame:
                where = frames.local_frame.index(var[1])
                value = frames.local_values[where]
                print(value, end='')
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54)
        if var[0] == "TF": 
            if var[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(var[1])
                value = frames.tmp_values[where]
                print(value, end='')
            else:
                print("Variable doesn't exists",file=sys.stderr)
                exit(54) 

def var_for_str(tmp_var):
    if tmp_var[0] == "GF":
        if tmp_var[1] in frames.global_frame:
            where = frames.global_frame.index(tmp_var[1])
            if frames.global_types[where] == "string":
                value = frames.global_values[where]
            else:
                print("Invalid type",file=sys.stderr)
                exit(53)  
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if tmp_var[0] == "LF":
        if var[1] in frames.local_frame:
            where = frames.local_frame.index(tmp_var[1])
            if frames.local_types[where] == "string":
                value = frames.local_values[where]
            else:
                print("Invalid type",file=sys.stderr)
                exit(53)
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if tmp_var[0] == "TF": 
        if tmp_var[1] in frames.tmp_frame:
            where = frames.tmp_frame.index(tmp_var[1])
            if frames.tmp_types[where] == "string":
                value = frames.tmp_values[where]
            else:
                print("Invalid type",file=sys.stderr)
                exit(53)
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54) 

    return value

def check_concat(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]  

    if symb1.type == "string" and symb2.type == "string":
        result = symb1.value + symb2.value   
    elif symb1.type == "string" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        value = var_for_str(tmp_var)
        result = symb1.value + value
    elif symb1.type == "var" and symb2.type == "string":
        tmp_var = symb1.value.split("@")
        value = var_for_str(tmp_var)
        result = value + symb2.value
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")

        value1 = var_for_str(tmp_var1)
        value2 = var_for_str(tmp_var2)

        result = value1 + value2
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    #zapis do var
    save_var(var, result, "string")


def check_strlen(instruction):
    var = instruction.args[0] 
    symb = instruction.args[1] #premenna(gf,lf,tf) alebo konstanta(string, int, nil, bool)

    if symb.type == "string":
      str_len = len(symb.value)  

    elif symb.type == "var":
        tmp_var = symb.value.split("@")
        if tmp_var[0] == "GF":
            if tmp_var[1] in frames.global_frame:
                where = frames.global_frame.index(tmp_var[1])
                if frames.global_types[where] == "string":
                    str_len = len(frames.global_values[where])
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53) 
        if tmp_var[0] == "LF":
            if tmp_var[1] in frames.local_frame:
                where = frames.local_frame.index(tmp_var[1])
                if frames.local_types[where] == "string":
                    str_len = len(frames.local_values[where])
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)     
        if tmp_var[0] == "TF": 
            if tmp_var[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(tmp_var[1])
                if frames.tmp_types[where] == "string":
                    str_len = len(frames.tmp_values[where])
                else:
                    print("Invalid type",file=sys.stderr)
                    exit(53)   
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)
    #zapis do var
    save_var(var, str_len, "int")

def check_getchar(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2] 

    if symb1.type == "string" and symb2.type == "int":
        str_len = len(symb1.value)
        if str_len < int(symb2.value):
            print("Invalid index",file=sys.stderr)
            exit(58) 
        else:
            letter = symb1.value[int(symb2.value)]
    elif symb1.type == "var" and symb2.type == "int":
        tmp_var = symb1.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)
        if tmp_type == "string":
            str_len = len(tmp_var)
            if str_len < int(symb2.value):
                print("Invalid index",file=sys.stderr)
                exit(58) 
            else:
                letter = tmp_var[int(symb2.value)]
        else:
            print("Invalid type", file=sys.stderr)
            exit(53)
    elif symb1.type == "string" and symb2.type == "var":
        tmp_var = symb2.value.split("@")
        tmp_type, tmp_var = var_type(tmp_var)
        str_len = len(symb1.value)
        if tmp_type == "int":
            if str_len < int(tmp_var):
                print("Invalid index",file=sys.stderr)
                exit(58) 
            else:
                letter = symb1.value[int(tmp_var)]
        else:
            print("Invalid type", file=sys.stderr)
            exit(53)
    elif symb1.type == "var" and symb2.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_var2 = symb2.value.split("@")
        tmp_type1, tmp_var1 = var_type(tmp_var1)
        tmp_type2, tmp_var2 = var_type(tmp_var2)

        if tmp_type1 == "string" and tmp_type2 == "int":
            str_len = len(tmp_var1)
            if str_len < int(tmp_var2):
                print("Invalid index",file=sys.stderr)
                exit(58) 
            else:
                letter = tmp_var1[int(tmp_var2)]
        else:
            print("Invalid type", file=sys.stderr)
            exit(53)
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    save_var(var, letter, "string")

def check_setchar(instruction):
    var = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2]

    if symb2.type == "string":
        letter = symb2.value[0]
    elif symb2.type == "var":
        tmp_var2 = symb2.value.split("@")
        tmp_type2, tmp_var2 = var_type(tmp_var2)
        if tmp_type2 == "string":
            letter = tmp_var2[0]
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)  
    else:
        print("Invalid type",file=sys.stderr)
        exit(53)

    if symb1.type == "int":
        position = int(symb1.value)    
    elif symb1.type == "var":
        tmp_var1 = symb1.value.split("@")
        tmp_type1, tmp_var1 = var_type(tmp_var1)
        if tmp_type1 == "int":
            position = int(tmp_var1)
        else:
            print("Invalid type",file=sys.stderr)
            exit(53)
    else: 
        print("Invalid type",file=sys.stderr)
        exit(53)
    
    var = var.value.split("@")
    if var[0] == "GF":
        if var[1] in frames.global_frame:    
            where = frames.global_frame.index(var[1])
            if frames.global_types[where] == "string":
                string = frames.global_values[where]
            else:
                print("Invalid type",file=sys.stderr)
                exit(53)  
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if var[0] == "LF":
        if var[1] in frames.local_frame:
            where = frames.local_frame.index(var[1])
            if frames.local_types[where] == "string":
                string = frames.local_values[where]
            else:
                print("Invalid type",file=sys.stderr)
                exit(53) 
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)
    if var[0] == "TF": 
        if var[1] in frames.tmp_frame:
            where = frames.tmp_frame.index(var[1])
            if frames.tmp_types[where] == "string":
                string = frames.tmp_values[where]
            else:
                print("Invalid type",file=sys.stderr)
                exit(53) 
        else:
            print("Variable doesn't exists",file=sys.stderr)
            exit(54)

    var = instruction.args[0]
    str_len = len(string)
    str_end = position + 1
    string = string[:position] + letter + string[str_end:]
    save_var(var, string, "string")

def check_type(instruction):
    var = instruction.args[0] 
    symb = instruction.args[1] #premenna(gf,lf,tf) alebo konstanta(string, int, nil, bool)

    d_type = symb.type
    save_type = ""

    if d_type == "bool":
        save_type = "bool"
    if d_type == "nil":
        save_type = "nil"
    if d_type == "int":
        save_type = "int"
    if d_type == "string":
        save_type = "string"

    if d_type == "var":
        tmp_var = symb.value.split("@")
        #todo ako premenna neexistuje tak prazdny retazec
        if tmp_var[0] == "GF":
            if not frames.global_frame:
                save_type = ""
            elif tmp_var[1] in frames.global_frame:
                where = frames.global_frame.index(tmp_var[1])
                save_type = frames.global_types[where] 
        if tmp_var[0] == "LF":
            if not frames.local_frame:
                save_type = ""
            elif var[1] in frames.local_frame:
                where = frames.local_frame.index(tmp_var[1])
                save_type = frames.local_types[where]    
        if tmp_var[0] == "TF": 
            if not frames.tmp_frame:
                save_type = ""
            elif tmp_var[1] in frames.tmp_frame:
                where = frames.tmp_frame.index(tmp_var[1])
                save_type = frames.tmp_types[where]  

    #zapis do var
    save_var(var, save_type, "string")           

def check_label(label, pos):
    if label.value not in labels.label_list:
        labels.label_list.append(label.value)
        labels.label_index.append(pos.number)
    else:
        print("Label already exists",file=sys.stderr)
        exit(52)

def save_call(label, pos):
    labels.call_list.append(label.value)
    labels.call_index.append(pos.number)

def check_jump(label):
    global pos
    if label.value not in labels.label_list:
        print("Label doesn't exists",file=sys.stderr)
        exit(52)
    else:
        label_pos = labels.label_list.index(label.value)
        position = labels.label_index[label_pos]
        pos = labels.label_index[label_pos]-1

def check_jumpifeq(instruction):
    label = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2] 
    
    result = eq(symb1, symb2)   
    if result == "true":
        check_jump(label)

def check_jumpifneq(instruction):
    label = instruction.args[0] #kam nacitam
    symb1 = instruction.args[1] #aky typ nacitam 
    symb2 = instruction.args[2] 
    
    result = eq(symb1, symb2)  
    if result == "false":
        check_jump(label)

def check_exit(symb):
    if symb.type == "bool":
        if symb.value == "true":
            exit(1)
        else:
            exit(2)
    elif symb.type == "int":
        ret = int(symb.value)
        if ret >= 0 and ret <= 49:
            exit(ret)
        else:
            print("Exit number is out of range",file=sys.stderr)
            exit(57)
    else:
        print("Invalid exit number",file=sys.stderr)
        exit(57)

def check_dprint(symb):
    d_type = symb.type
    if d_type in ("int", "string", "nil", "bool"):
        print(symb.value,file=sys.stderr)

    symb = symb.value.split("@")
    if symb[0] in ("GF", "LF", "TF"):
        print(symb[1],file=sys.stderr)


#analyza jednotlivych instrukcii
def opcode(instruction):
    global inst_count
    #opcode = instruction.attrib['opcode'].upper()
    opcode = instruction.name

    if opcode == 'MOVE':
        check_move(instruction)
    elif opcode == 'CREATEFRAME':
        check_createframe()
    elif opcode == 'PUSHFRAME':
        check_pushframe()
    elif opcode == 'POPFRAME':
        check_popframe()
    elif opcode == 'DEFVAR':
        check_defvar(instruction.args[0])
    elif opcode == 'CALL':
        check_call(instruction.args[0])
    elif opcode == 'RETURN':
        check_return()
    elif opcode == 'PUSHS':
        check_pushs(instruction.args[0])
    elif opcode == 'POPS':
        check_pops(instruction.args[0])
    elif opcode == 'ADD':
        check_add(instruction)
    elif opcode == 'SUB':
        check_sub(instruction)
    elif opcode == 'MUL':
        check_mul(instruction)
    elif opcode == 'IDIV':
        check_idiv(instruction)
    elif opcode == 'LT':
        check_lt(instruction)
    elif opcode == 'GT':
        check_gt(instruction)
    elif opcode == 'EQ':
        check_eq(instruction)
    elif opcode == 'AND':
        check_and(instruction)
    elif opcode == 'OR':
        check_or(instruction)
    elif opcode == 'NOT':
        check_not(instruction)
    elif opcode == 'INT2CHAR':
        check_int2char(instruction)
    elif opcode == 'STRI2INT':
        check_stri2int(instruction)
    elif opcode == 'READ':
        check_read(instruction)
    elif opcode == 'WRITE':
        check_write(instruction.args[0])
    elif opcode == 'CONCAT':
        check_concat(instruction)
    elif opcode == 'STRLEN':
        check_strlen(instruction)
    elif opcode == 'GETCHAR':
        check_getchar(instruction)
    elif opcode == 'SETCHAR':
        check_setchar(instruction)
    elif opcode == 'TYPE':
        check_type(instruction)
    elif opcode == 'LABEL':
        pass
    elif opcode == 'JUMP':
        check_jump(instruction.args[0])
    elif opcode == 'JUMPIFEQ':
        check_jumpifeq(instruction)
    elif opcode == 'JUMPIFNEQ':
        check_jumpifneq(instruction)
    elif opcode == 'EXIT':
        check_exit(instruction.args[0])
    elif opcode == 'DPRINT':
        check_dprint(instruction.args[0])
    elif opcode == 'BREAK':
        print("The number of executed instructions is: ", inst_count, file=stderr)
    else:
        print("Invalid instruction", file=sys.stderr)

#zaciatok programu
#kontrola zadanych parametrov
if len(sys.argv) > 2:
    print("Too many arguments")
    exit(10)

parser = argparse.ArgumentParser(description='Interpret of IPPcode22 language')
parser.add_argument("--source", nargs=1, help='File to read from')
parser.add_argument("--input", nargs=1, help='File that will take the entrance for IPPcode22')
args = vars(parser.parse_args())

source_file = None
input_file = None
#priradenie/otvorenie spravnych suborov podla argumentu
#todo prerobit
if args["source"] is None and args["input"] is None:
    print("Missing soure or input file", file=sys.stderr)
    exit(10)
if args['source']:
    source_file = args['source'][0]
if args['input']:
    input_file = open(args['input'], "r")


#xml load
tree = None
try:
  if source_file:
    tree = ET.parse(source_file)
  else:
    tree = ET.parse(stdin)
except Exception as e:
    print("Wrong XML formation", file=sys.stderr)
    exit(31)

#check xml structure
root = tree.getroot()

if root.tag != "program":
    print("Wrong root tag", file=sys.stderr)
    exit(32)

root_attrib = root.attrib
#todo prerobit tento try
try:
    if root_attrib["language"].upper() != "IPPCODE22":
        print("Wrong language",file=sys.stderr)
        exit(31) 
except:
    print("Missing attribute: language",file=sys.stderr)
    exit(32)  

instruction = list()
inst_count = 0
instCount = 1

prev_order = 0
for child in root:
    if child.tag != "instruction":
        print("Invalid format",file=sys.stderr)
        exit(32)  
    if 'order' not in child.attrib: 
        print("Missing order attribute",file=sys.stderr)
        exit(32)
    if 'opcode' not in child.attrib:
        print("Missing opcode attribute",file=sys.stderr)
        exit(32)

    #todo urobit osetredie kde je order s opakujucim sa cislom
    if prev_order == child.attrib['order']:
        print("Same",file=sys.stderr)
        exit(32)
    else:
         prev_order = child.attrib['order']
    #todo prepisat
    for arg in child:
        # Kontrola jednoho atributu "type"
        #?? do i need this
        if (len(arg.attrib) != 1):
            exit(32)
        if not(re.fullmatch("arg[1|2|3]",arg.tag)):
            print("Invalid number of argument")
            exit(32)

    #instruction.append(child)

    #todo neviem co robi ale skusam ako s tym pracovat
    instruction.append(Instruction(child.attrib['opcode'].upper(), instCount))

    for subelem in child:
        instruction[instCount-1].addArgument(subelem.attrib['type'].lower(), subelem.text)
    instCount += 1

for i in instruction:
    if i.name == "LABEL":
        check_label(i.args[0], i)
    if i.name == "CALL":
        save_call(i.args[0], i)
#todo prerobit
program_length = (len(instruction))
pos = 0
while pos < program_length:
    opcode(instruction[pos])
    pos += 1
    inst_count +=1

#print(" ", frames.global_frame)
#print(" ", frames.global_values)
#print(" ", frames.global_types)
#print(" ", frames.stack)
#print("\n")
#print(inst_count)
#print(" ", frames.stack)
#print(" ", frames.stack_types)