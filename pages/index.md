$title: Hello$Goal:
[A no bullshit systems programming](https://www.google.com) language that improves upon C while still feeling low level

structured programming (if, while, scopes)
no hidden complexity
closely reflects hardware
made for x64

? multipointer, dyn array, ...
```c
&type     - pointer, pointer to * elements
[]type    - slice,   pointer to N(mutable) elements, convenience ptr + length
[N]type   - array,   pointer to N(immutable) elements
```

? overflow, bounds checking, implicit casting
```c
ident, 0, "", '', true, proc, struct, union, enum, .[], .{}, .(), .b, (expr)
(), [N], [N:N], a.b, ^
[], [N], ^
a.[], a.{}, a.()
+, -, ~, !
.., ..<
*, /, %, &, >>, >>>, <<, <<<
+, -, |, ~
// TODO: Moare
//// ERROR
/// HI
/// TODO: HI
==, !=, <, >, <=, >=
&&
||
:?
	
	
""   - utf8 string
''   - ASCII character
%    - truncated division remainder
<<   - logical right shift
>>>  - arithmetic right shift
a.b  - auto deref member access
all shifts are masked

maybe
<<< - splat bits to left YES, >>> is already kind of arbitrary, and <<< is usefull
u32 wraps on overflow, uint traps
%+ is for wrapping addition, + is for trapping addition
```

? Polymorphism
```c
proc(args) -> ret_val { body }
proc(args) -> (name: ret_type, name: ret_type)  { body }
struct { body }
union { body }
enum { body }
```

```c
:label if (init; condition) statement
when (init; condition) statement
else statement
:label while (init; condition; step) statement
:label { block }
defer statement
return .a = 0, .b = 1
using expr as alias
break label
continue label
a ?= b
```

```c
a, b, c: a = a, b, c; // ---
a, b, c :: a, b, c;
include "file" as alias
```





Should: using expr as expr. Why not
using expr
convenience, may be better for aliasing than 
a: struct { x, y: int; };
using a;
x = 0;
y = 1;

combined with auto deref gives abstraction over structure without affecting usage
```c
Pos :: struct
{
	x, y, z: int;
};

p: Pos;
print(p.x, p.y, p.z);
```
```c
Pos_Cold :: struct
{
	x, y: int;
};

Pos :: struct
{
	using cold: ^Pos_Cold;
	z: int;
};

cold: Pos_Cold;
p: Pos.{ cold = &cold, z = 1 };
print(p.x, p.y, p.z);
```
allows assigning to multiple fields without affecting usage

using expr as expr



return assignment and aggregate init assignment syntax
either assignment is an expression and sema roots out illegal uses or a = b cannot be the syntax used
alternate syntaxes
Just stick with a = b, but do it only in cases where it is legal, disjoint representation from =




Polymorhism
Macros
Compile Time Execution



Metaprogramming

pointer syntax: is this still a problem?
```c
^int, ^x
&int, &x
```
problems:
- cannot discern between pointer and pointer type
```
^int.(0) // ambigous: pointer_to_int.(0) or pointer_to(int.(0))
```

This should probably only affect parsing and typeid
```
a: typeid = 0;
b: ^typeid = 0;
c := ^a; // is this a pointer type or a pointer to a typeid

A :: typeid.(0);
C :: ^A;

```

Pros: intuitive syntax
Cons: requires type info to discern references from pointer types

```c
transmute(f32) (transmute(u32) f & (1 << 31))
	
transmute(f32, transmute(u32, f) & (1 << 31))
	
	
*cast(&int, targa_header.int_something[6])
```

stick with ^ and &




# Type System
### primitives
- byte : a single byte with no extra information about what it represents
- int : signed integer with unspecified size (used for general computation where the size is not important, i.e. the intention is normal arith, not modular arith)
- i8,i16,i32,i64,i128 : signed integer with fixed size
- uint : unsigned integer with unspecified size
- u8,u16,u32,u64,u128 : unsigned integer with fixed size
- float : IEEE 754 floating point with unspecified size
- f16,f32,f64 : IEEE 754 float with specified size
- string : length based string (fat pointer)
- bool : boolean value with unspecified size
- b8,b16,b32,b64,b128: boolean value with specified size

### base
- type info
- any (byte pointer, type info)

### Composite
- pointer
- array
- slice
- struct, union
- enum
- proc

Types are "first class", every type has an ID, most types have a "type info"

It would be nice to be able to serialize a struct and attach the typeid, then deserialize it by using the typeid and type info, but that would require quite stable typeids, it would be really nice if typeids were UUIDS, since typeids could then cross dll boundaries and be serialized, however, that would at least cause some overhead when looking up type information and would be of limited value without the type info


remove typeid, types can only be compared with == when they are constant, typeinfos can be compared with a "deep compare" or == (may be 'unsafe')

All types have a type info struct, types function as normal until they are bound the a non constant value, then they are cast to a type info pointer.


&a.b => &(a.b)
&a.{as}

```c
uint, struct {}

typeid ==
&type_info == + info

// passing type to proc
Make :: proc($T) -> &T
{
    return 0;
}

// sizeof
sizeof(uint)

a := 0;
b : typeof(a) = 0;

sizeof(typeof(a))

typeof returns a type, which is converted to a &type_info when losing constant status

&typeof(a).(b)
&T.(a)
```

type specialization as in Odin


type <=> type_info
a: type; // illegal


A :: &type.(0);

a: A;

// creating a variable of type type is illegal
// dereferencing the type is illegal
// cast to &type is illegal

```c
Make :: proc($T: type = uint) -> &T
{
	type_info_of(T)
}
```


problems with Odin's approach
- mostly useless typeid, type_info has a superset of the functionality
- typeids cannot cross dll boundary
- typeids cannot be stored on disk

storing typeids on disk is  probably a mistake
typeids crossing dll boundaries is useful
comparing typeids with == makes that error prone
comparing typeids with a function that uses == when both typeids are from the same program, and some other comparison if they are not, is probably safer

solution 1: ditch typeid, use &type_info. Disallow creating type_infos and casting to type_info (at least in a constant context)
problems: weird,

uint, struct {}

a: type;



```c
Make :: proc($T: typeid/[]$E) -> &T
{
}
```



cross dll boundary (network?)
type id + information accessible at runtime

problems with Odin's approach
