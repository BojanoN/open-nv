vec_types = {"vec2" : 2, "vec3" : 3, "vec4" : 4}
ops = {"add" : '+', "sub" : '-',"mul" : '*',"div" : '/'}



def op_func_declare(type, op):
    return "inline void {}_{}({} a, {} b, {} dst)".format(type, op, type, type, type, type) 

def op_const_func_declare(type, op):
    return "inline void {}_{}c({} a,  {} dst, float val)".format(type, op, type, type, type, type)

def dot_declare(type):
    return "inline float {}_dot({} a, {} b)".format(type, type, type)

def norm_declare(type):
    return "inline float {}_norm({} a)".format(type, type)

def normalize_declare(type):
    return " inline void {}_normalize({} a)".format(type, type)

def cross_func_declare():
    return "inline void vec3_cross(vec3 a,  vec3 b, vec3 dst)"


def vectype_hpp_gen(type):
    ret = '#pragma once\n#include "types.hpp"\n\n'

    ret += "inline void {}_init(float* in, {} vec);\n".format(type, type)

    for key, val in ops.items():
        ret += op_func_declare(type, key) +"; \n" + op_const_func_declare(type, key) +"; \n"

    ret += dot_declare(type) + "; \n"
    ret += norm_declare(type) + "; \n"
    ret += normalize_declare(type) + "; \n"

    if type == "vec3":
        ret += cross_func_declare()+"; \n"


    with open(type+".hpp", "w") as file:
        file.write(ret)


def define_vec_ops(type):
    ret = ""
    for key, val in ops.items():
        ret += "inline void {}_{}({} a, {} b, {} dst)".format(type, key, type, type, type, type)  +"{ \n"
        for i in range(0, vec_types[type]):
            ret += "dst[{}] = a[{}] {} b[{}];\n".format(i, i, val, i)
        ret += "}\n"

        ret += "inline void {}_{}c({} a,  {} dst, float val)".format(type, key, type, type, type, type) +"{ \n"
        for i in range(0, vec_types[type]):
            ret += "dst[{}] = a[{}] {} val;\n".format(i, i, val)
        ret += "}\n"

    return ret

def dot_define(type):
    ret = "inline float {}_dot({} a, {} b)".format(type, type, type)
    ret += "{\n return "

    for i in range(vec_types[type]):
        ret += "a[{}] * b[{}] ".format(i, i)
        if i+1 != vec_types[type]:
            ret += "+"
    ret += ";\n"

    ret += "}\n"
    return ret

def norm_define(type):
    ret = "inline float {}_norm({} a)".format(type, type)
    ret += "{\n return sqrt("

    for i in range(vec_types[type]):
        ret += "a[{}] * a[{}] ".format(i, i)
        if i+1 != vec_types[type]:
            ret += "+"
    ret += ");\n"
    ret += "}\n"

    return ret;

def normalize_define(type):
    ret = "inline void {}_normalize({} a)".format(type, type)
    ret += "{\n"

    ret += "float norm = {}_norm(a);\n".format(type)
    for i in range(vec_types[type]):
        ret += "a[{}] /= norm;\n".format(i)

    ret += "}\n"

    return ret

def cross_func_define():
    return "inline void vec3_cross(vec3 a,  vec3 b, vec3 dst){\n dst[0] = a[1]*b[2] - b[1]*a[2];\ndst[1] = a[2]*b[0] - b[2]*a[0];\ndst[2] = a[0]*b[1] - b[0]*a[1];\n}\n"

def vectype_cpp_gen():
    ret=""
    for key, value in vec_types.items():
        ret = '#include <math.h>\n#include<stdio.h>\n'.format(key)
        ret += "inline void {}_init(float* in, {} vec)".format(key, key)
        ret += "{\n"
        for i in range(value):
            ret += "vec[{}] = in[{}];\n".format(i, i)
        ret += "}\n"

        ret += "inline void {}_print({} vec)".format(key, key)
        ret += '{\nprintf("( '+"%f "*value + ')\\n",'+"".join("vec[{}], ".format(x) for x in range(value-1)) + 'vec[{}] );\n'.format(value-1)
        ret += "}\n"


        ret += define_vec_ops(key)+"\n"

        ret += dot_define(key)+"\n"
        ret += norm_define(key)+"\n"
        ret += normalize_define(key)+"\n"

        if key == "vec3":
            ret += cross_func_define()+"\n"

        with open(key+".hpp", "w") as file:
            file.write(ret)

        ret = ""

#for key, val in vec_types.items():
#    vectype_hpp_gen(key)
vectype_cpp_gen()
