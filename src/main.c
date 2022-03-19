#define _CRT_SECURE_NO_WARNINGS

#define NOMINMAX            1
#define WIN32_LEAN_AND_MEAN 1
#define WIN32_MEAN_AND_LEAN 1
#define VC_EXTRALEAN        1
#include <windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN
#undef WIN32_MEAN_AND_LEAN
#undef VC_EXTRALEAN
#undef far
#undef near

#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

typedef signed __int8  i8;
typedef signed __int16 i16;
typedef signed __int32 i32;
typedef signed __int64 i64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef u64 umm;
typedef i64 imm;

typedef float  f32;
typedef double f64;

typedef u8 bool;

typedef struct String
{
    u8* data;
    u64 size;
} String;

typedef struct Growable_String
{
    u8* data;
    u64 size;
    u64 capacity;
} Growable_String;

#define STRING(str) (String){ .data = (u8*)(str), .size = sizeof(str) - 1 }

#define true 1
#define false 0

#define U8_MAX  0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFF

#define I8_MIN  0xFF
#define I16_MIN 0xFFFF
#define I32_MIN 0xFFFFFFFF
#define I64_MIN 0xFFFFFFFFFFFFFFFF

#define I8_MAX  0x7F
#define I16_MAX 0x7FFF
#define I32_MAX 0x7FFFFFFF
#define I64_MAX 0x7FFFFFFFFFFFFFFF

#define ASSERT(EX) ((EX) ? 1 : (*(volatile int*)0 = 0))
#define NOT_IMPLEMENTED ASSERT(!"NOT_IMPLEMENTED")
#define INVALID_DEFAULT_CASE default: ASSERT(!"INVALID_DEFAULT_CASE"); break
#define INVALID_CODE_PATH ASSERT(!"INVALID_CODE_PATH")

#define OFFSETOF(element, type) (umm)&((type*)0)->element
#define ALIGNOF(T) OFFSETOF(t, struct { u8 b; T t; })

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(n) ((n) < 0 ? -(n) : (n))
#define SGN(n) ((n) < 0 ? -1 : ((n) == 0 ? 0 : 1))

#define KB(N) ((umm)(N) << 10)
#define MB(N) ((umm)(N) << 20)
#define GB(N) ((umm)(N) << 30)
#define TB(N) ((umm)(N) << 40)

#define IS_POW_OF_2(n) (((n == 0) | ((n) & ((n) - 1))) == 0)

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define internal static
#define global static

// NOTE: This is just a hack to work around a parsing bug in 4coder
#define TYPEDEF_FUNC(return_val, name, ...) typedef return_val (*name)(__VA_ARGS__)

bool
String_Match(String s0, String s1)
{
    bool result = true;
    
    if (s0.size != s1.size) result = false;
    else
    {
        for (umm i = 0; i < s0.size; ++i)
        {
            if (s0.data[i] != s1.data[i])
            {
                result = false;
                break;
            }
        }
    }
    
    return result;
}

void
GeneratePreamble(FILE* out, String title, umm depth)
{
    fprintf(out, "<!doctype html>\n<html>\n<head>\n");
    fprintf(out, "<title>%.*s</title>\n", (int)title.size, title.data);
    fprintf(out, "<link rel=\"stylesheet\" href=\"");
    for (umm j = 0; j < depth; ++j) fprintf(out, "../");
    fprintf(out, "main.css\">\n</head>\n<body>\n");
    fprintf(out, "<div id=\"content\">\n");
    fprintf(out, "<div id=\"navbar\">\n");
    fprintf(out, "<a id=\"logo\" href=\"");
    for (umm j = 0; j < depth; ++j) fprintf(out, "../");
    fprintf(out, "index.html\">\n");
    fprintf(out, "<svg width=\"9.2622mm\" height=\"9.2616mm\" version=\"1.1\" viewBox=\"0 0 9.2622 9.2616\" xmlns=\"http://www.w3.org/2000/svg\">"
            " <g transform=\"translate(58.868 -264.01)\">"
            "  <path transform=\"scale(.26458)\" d=\"m-221.53 998.06c-0.4155 0-0.75 0.33452-0.75 0.75v33.07c0 0.4155 0.33451 0.75 0.75 0.75h33.072c0.4155 0 0.75-0.3345 0.75-0.75v-33.07c0-0.41552-0.33451-0.75-0.75-0.75zm0.17968 3.7402h13.076v0.7227c-1.224 0.1171-2.0182 0.3253-2.3828 0.625-0.36458 0.2994-0.54687 0.6836-0.54687 1.1523 0 0.6511 0.29948 1.6667 0.89844 3.0469l6.7383 15.527 6.25-15.332c0.61197-1.5104 0.91797-2.5586 0.91797-3.1445 0-0.3776-0.1888-0.7356-0.56641-1.0742-0.3776-0.3516-1.0156-0.599-1.9141-0.7422-0.0651-0.013-0.17579-0.032-0.33203-0.059h-0.77149v-0.7227h11.346v0.7227h-0.95703c-1.0807 0-1.8685 0.319-2.3633 0.957-0.32551 0.4166-0.48633 1.4193-0.48633 3.0078v17.109c0 1.3411 0.0846 2.2266 0.25391 2.6563 0.13022 0.3254 0.4017 0.6055 0.81836 0.8398 0.55989 0.3126 1.1524 0.4687 1.7773 0.4687h0.95703v0.7227h-11.346v-0.7227h0.9375c1.0938 0 1.888-0.319 2.3828-0.957 0.31251-0.4166 0.46875-1.4193 0.46875-3.0078v-14.363l-8.0566 19.656h-0.72265l-8.8379-20.338v15.045c0 1.3411 0.0846 2.2266 0.2539 2.6563 0.13022 0.3254 0.40367 0.6055 0.82031 0.8398 0.55991 0.3126 1.1524 0.4687 1.7774 0.4687h0.95703v0.7227h-11.348v-0.7227h0.9375c1.0938 0 1.888-0.319 2.3828-0.957 0.3125-0.4166 0.46875-1.4193 0.46875-3.0078v-17.109c0-1.3411-0.0846-2.2265-0.25391-2.6562-0.13022-0.3255-0.39715-0.6055-0.80078-0.8399-0.57292-0.3126-1.1719-0.4687-1.7969-0.4687h-0.9375z\" stroke-width=\".43432\"/>"
            " </g>"
            "</svg></a>"
            );
    fprintf(out, "\n<ul>\n"
            "<li><a href=\"");
    for (umm j = 0; j < depth; ++j) fprintf(out, "../");
    fprintf(out, "log_index.html\">devlogs</a></li>\n"
            "<li class=\"navbar_li_divider\">|</li>\n"
            "<li><a href=\"");
    for (umm j = 0; j < depth; ++j) fprintf(out, "../");
    fprintf(out, "doc_index.html\">docs</a></li>\n"
            "<li class=\"navbar_li_divider\">|</li>\n"
            "<li><a href=\"https://github.com/Soimn/mm\">GitHub</a></li></ul>\n");
    fprintf(out, "</div>\n");
}

void
GeneratePostamble(FILE* out)
{
    fprintf(out, "</div>\n<div id=\"footer\"></div>\n</body>\n</html>");
}

FILE* LogIndex = 0;
bool
GenerateFile(String contents, String filename, FILE* out, umm depth, bool is_devlog)
{
    bool encountered_errors = false;
    
    umm line_start = 0;
    umm line       = 1;
    
    String title = filename;
    String date  = STRING("");
    
    umm i = 0;
    if (contents.size != 0 && contents.data[0] == '$')
    {
        i += 1;
        
        for (; i < contents.size && contents.data[i] != '$' && !encountered_errors; )
        {
            if (contents.data[i] == '\n' || i + 1 < contents.size && contents.data[i] == '\r' && contents.data[i + 1] == '\n')
            {
                i += 1 + (contents.data[i] == '\r');
                line_start = i;
                line      += 1;
            }
            else
            {
                while (i < contents.size && (contents.data[i] == ' '  || contents.data[i] == '\t' ||
                                             contents.data[i] == '\r' || contents.data[i] == '\n')) i += 1;
                
                String argument = { .data = contents.data + i, .size = 0 };
                while (i < contents.size && (contents.data[i] >= 'a' && contents.data[i] <= 'z' ||
                                             contents.data[i] >= 'A' && contents.data[i] <= 'Z' ||
                                             contents.data[i] >= '0' && contents.data[i] <= '9' ||
                                             contents.data[i] == '_'))
                {
                    ++i;
                    ++argument.size;
                }
                
                if (String_Match(argument, STRING("title")))
                {
                    if (i == contents.size || contents.data[i] != ':')
                    {
                        //// ERROR:
                        fprintf(stderr, "ERROR(%llu:%llu): Missing colon after 'title' config argument\n", line, i - line_start);
                        encountered_errors = true;
                    }
                    else
                    {
                        i += 1;
                        
                        while (i < contents.size && (contents.data[i] == ' ' || contents.data[i] == '\t')) i += 1;
                        
                        String value = { .data = contents.data + i, .size = 0 };
                        while (i < contents.size && contents.data[i] != '\r' && contents.data[i] != '\n')
                        {
                            ++i;
                            ++value.size;
                        }
                        
                        if (value.size == 0)
                        {
                            //// ERROR:
                            fprintf(stderr, "ERROR(%llu:%llu): Missing value of 'title' config argument\n", line, i - line_start);
                            encountered_errors = true;
                        }
                        else
                        {
                            title = value;
                        }
                    }
                }
                else if (String_Match(argument, STRING("date")))
                {
                    if (i == contents.size || contents.data[i] != ':')
                    {
                        //// ERROR:
                        fprintf(stderr, "ERROR(%llu:%llu): Missing colon after 'date' config argument\n", line, i - line_start);
                        encountered_errors = true;
                    }
                    else
                    {
                        i += 1;
                        
                        while (i < contents.size && (contents.data[i] == ' ' || contents.data[i] == '\t')) i += 1;
                        
                        if (i + sizeof("yyyy.mm.dd") - 1 > contents.size)
                        {
                            //// ERROR:
                            fprintf(stderr, "ERROR(%llu:%llu): Missing iso date after 'date' argument\n", line, i - line_start);
                            encountered_errors = true;
                        }
                        else if (contents.data[i + 0] < '0' && contents.data[i + 0] > '9' ||
                                 contents.data[i + 1] < '0' && contents.data[i + 1] > '9' ||
                                 contents.data[i + 2] < '0' && contents.data[i + 2] > '9' ||
                                 contents.data[i + 3] < '0' && contents.data[i + 3] > '9' ||
                                 contents.data[i + 4] != '.'                              ||
                                 contents.data[i + 5] < '0' && contents.data[i + 5] > '9' ||
                                 contents.data[i + 6] < '0' && contents.data[i + 6] > '9' ||
                                 contents.data[i + 7] != '.'                              ||
                                 contents.data[i + 8] < '0' && contents.data[i + 8] > '9' ||
                                 contents.data[i + 9] < '0' && contents.data[i + 9] > '9')
                        {
                            //// ERROR:
                            fprintf(stderr, "ERROR(%llu:%llu): Invalid iso date\n", line, i - line_start);
                            encountered_errors = true;
                        }
                        else
                        {
                            date = (String){ .data = contents.data + i, .size = sizeof("yyyy.mm.dd") - 1 };
                            i += sizeof("yyyy.mm.dd") - 1;
                        }
                    }
                }
                else
                {
                    //// ERROR:
                    fprintf(stderr, "ERROR(%llu:%llu): Unknown config argument\n %s", line, i - line_start, contents.data + i);
                    encountered_errors = true;
                }
            }
        }
        
        if (!encountered_errors && (i == contents.size || contents.data[i] != '$'))
        {
            //// ERROR:
            fprintf(stderr, "ERROR(%llu:%llu): Missing $ after configuration\n", line, i - line_start);
            encountered_errors = true;
        }
        else i += 1;
    }
    
    GeneratePreamble(out, title, depth);
    
    if (is_devlog)
    {
        fprintf(LogIndex, "<div class=\"devlog\"><div class=\"devlog_filename\">%.*s</div><div class=\"devlog_title\">%.*s</div></div>", (int)filename.size, filename.data, (int)title.size, title.data);
    }
    
    fprintf(out, "<h1 id=\"title\">%.*s</h1>", (int)title.size, title.data);
    if (date.size != 0) fprintf(out, "\n<p id=\"date\">%.*s</p>",  (int)date.size, date.data);
    
    umm heading_level = 0;
    umm decor_level   = 0;
    umm open_decors   = 0;
    for (; i < contents.size && !encountered_errors; )
    {
        if      (contents.data[i] == '\r') ++i;
        else if (contents.data[i] == '\\')
        {
            if (i + 1 >= contents.size)
            {
                //// ERROR:
                fprintf(stderr, "ERROR(%llu:%llu): Missing character after backslash\n", line, i - line_start + 1);
                encountered_errors = true;
            }
            else
            {
                fputc(contents.data[i + 1], out);
                i += 2;
            }
        }
        else if (contents.data[i] == '\n')
        {
            if (i + 1 < contents.size && contents.data[i + 1] == '\n' || 
                i + 2 < contents.size && contents.data[i + 1] == '\r' && contents.data[i + 2] == '\n')
            {
                if (contents.size && contents.data[i] != '\n') i += 1;
                
                if (decor_level != 0)
                {
                    if ((open_decors & 1) != 0) fprintf(out, "</span>");
                    if ((open_decors & 2) != 0) fprintf(out, "</span>");
                    if ((open_decors & 4) != 0) fprintf(out, "</span>");
                    decor_level = 0;
                    open_decors = 0;
                }
                
                fprintf(out, "\n<br>");
                
                i += 2;
            }
            
            if (heading_level != 0) fprintf(out, "</h%llu>", heading_level), heading_level = 0;
            
            fputc(contents.data[i++], out);
            line_start = i;
            line      += 1;
        }
        else if (contents.data[i] == '#')
        {
            umm j = i + 1;
            while (j < contents.size && contents.data[j] == '#') ++j;
            
            if (j >= contents.size || contents.data[j] != ' ')
            {
                //// ERROR:
                fprintf(stderr, "ERROR(%llu:%llu): Missing space after heading prefix\n", line, j - line_start + 1);
                encountered_errors = true;
            }
            else
            {
                heading_level = j - i + 1;
                i = j + 1;
                
                fprintf(out, "<h%llu>", heading_level);
            }
        }
        else if (contents.data[i] == '*')
        {
            umm j = i + 1;
            while (j < contents.size && contents.data[j] == '*') ++j;
            
            umm level = j - i;
            
            if (level > 3)
            {
                //// ERROR:
                fprintf(stderr, "ERROR(%llu:%llu): Too many decors\n", line, j - line_start + 1);
                encountered_errors = true;
            }
            else
            {
                bool is_closing = (decor_level == level);
                if      (level == 1) fprintf(out, (is_closing ? "</span>" : "<span class=\"i_text\">"));
                else if (level == 2) fprintf(out, (is_closing ? "</span>" : "<span class=\"b_text\">"));
                else if (level == 3) fprintf(out, (is_closing ? "</span>" : "<span class=\"bi_text\">"));
                
                if (is_closing) open_decors &= ~((umm)1 << (level - 1));
                else            open_decors |= (umm)1 << (level - 1);
                
                decor_level = level;
                
                i = j;
            }
        }
        else if (i + 2 < contents.size && contents.data[i] == '`' && contents.data[i + 1] == '`' && contents.data[i + 2] == '`')
        {
            i += 3;
            
            if (decor_level != 0)
            {
                if ((open_decors & 1) != 0) fprintf(out, "</span>");
                if ((open_decors & 2) != 0) fprintf(out, "</span>");
                if ((open_decors & 4) != 0) fprintf(out, "</span>");
                decor_level = 0;
                open_decors = 0;
            }
            
            fprintf(out, "<div class=\"code\">");
            
            while (i < contents.size && contents.data[i] != '\n') i += 1;
            if (i < contents.size && contents.data[i] == '\n') i += 1;
            
            if (i != contents.size)
            {
                fputc(contents.data[i++], out);
                line_start = i;
                line      += 1;
            }
            
            while (i < contents.size && !encountered_errors)
            {
                if (i + 2 < contents.size && contents.data[i] == '`' && contents.data[i + 1] == '`' && contents.data[i + 2] == '`')
                {
                    break;
                }
                else if (contents.data[i] >= 'a' && contents.data[i] <= 'z' ||
                         contents.data[i] >= 'A' && contents.data[i] <= 'Z' ||
                         contents.data[i] == '_')
                {
                    String keywords[] = {
                        STRING("if"),
                        STRING("while"),
                        STRING("for"),
                        STRING("proc"),
                        STRING("struct"),
                        STRING("union"),
                        STRING("enum"),
                        STRING("i8"),
                        STRING("i16"),
                        STRING("i32"),
                        STRING("i64"),
                        STRING("u8"),
                        STRING("u16"),
                        STRING("u32"),
                        STRING("u64"),
                        STRING("i8"),
                        STRING("i16"),
                        STRING("i32"),
                        STRING("bool"),
                        STRING("int"),
                        STRING("uint"),
                        STRING("f32"),
                        STRING("f64"),
                        STRING("float"),
                        STRING("using"),
                        STRING("defer"),
                        STRING("return"),
                        STRING("include"),
                        STRING("as"),
                        STRING("import"),
                        STRING("break"),
                        STRING("continue"),
                        STRING("when"),
                        STRING("else"),
                    };
                    
                    String identifier = { .data = contents.data + i, .size = 0 };
                    while (i < contents.size && (contents.data[i] >= 'a' && contents.data[i] <= 'z' ||
                                                 contents.data[i] >= 'A' && contents.data[i] <= 'Z' ||
                                                 contents.data[i] >= '0' && contents.data[i] <= '9' ||
                                                 contents.data[i] == '_'))
                    {
                        ++i;
                        ++identifier.size;
                    }
                    
                    umm k = 0;
                    for (; k < ARRAY_SIZE(keywords); ++k)
                    {
                        if (String_Match(identifier, keywords[k]))
                        {
                            break;
                        }
                    }
                    
                    if (String_Match(identifier, STRING("true")) || String_Match(identifier, STRING("false")))
                    {
                        fprintf(out, "<span class=\"code_bool\">%.*s</span>", (int)identifier.size, identifier.data);
                    }
                    else
                    {
                        if (k != ARRAY_SIZE(keywords)) fprintf(out, "<span class=\"code_keyword\">");
                        fprintf(out, "%.*s", (int)identifier.size, identifier.data);
                        if (k != ARRAY_SIZE(keywords)) fprintf(out, "</span>");
                    }
                }
                else if (contents.data[i] >= '0' && contents.data[i] <= '9')
                {
                    fprintf(out, "<span class=\"code_number\">");
                    
                    umm base = 10;
                    if (i + 1 < contents.size && contents.data[i] == '0')
                    {
                        if      (contents.data[i + 1] == 'x') base = 16;
                        else if (contents.data[i + 1] == 'h') base = 16;
                        else if (contents.data[i + 1] == 'd') base = 12;
                        else if (contents.data[i + 1] == 'o') base = 8;
                        else if (contents.data[i + 1] == 'b') base = 2;
                        
                        if (base != 10)
                        {
                            fprintf(out, "%.*s", 2, contents.data + i);
                            i += 2;
                        }
                    }
                    
                    while (i < contents.size)
                    {
                        u8 digit = 0xFF;
                        if      (contents.data[i] >= '0' && contents.data[i] <= '9') digit = contents.data[i] & 0xF;
                        else if (contents.data[i] >= 'A' && contents.data[i] <= 'Z') digit = 9 + (contents.data[i] & 0x1F);
                        else if (contents.data[i] >= 'a' && contents.data[i] <= 'z') digit = 25 + (contents.data[i] & 0x1F);
                        
                        if (digit >= base) break;
                        else fputc(contents.data[i++], out);
                    }
                    
                    fprintf(out, "</span>");
                }
                else if (i + 1 < contents.size && contents.data[i] == '/' && contents.data[i + 1] == '/')
                {
                    char* code_class = "code_comment";
                    if (i + 3 < contents.size && contents.data[i + 2] == '/' && contents.data[i + 3] == ' ')
                    {
                        code_class = "code_hi_comment";
                    }
                    else if (i + 4 < contents.size && contents.data[i + 2] == '/' && contents.data[i + 3] == '/' && 
                             contents.data[i + 4] == ' ')
                    {
                        code_class = "code_err_comment";
                    }
                    
                    fprintf(out, "<span class=\"%s\">", code_class);
                    while (i < contents.size && contents.data[i] == '/') fputc(contents.data[i++], out);
                    
                    if (i + sizeof("TODO") < contents.size && contents.data[i] == ' ' &&
                        String_Match((String){ .data = contents.data + i + 1, .size = sizeof("TODO") - 1}, STRING("TODO")))
                    {
                        fprintf(out, "<span class=\"code_todo\">TODO</span>");
                        i += sizeof("TODO");
                    }
                    else if (i + sizeof("NOTE") < contents.size && contents.data[i] == ' ' &&
                             String_Match((String){ .data = contents.data + i + 1, .size = sizeof("NOTE") - 1}, STRING("NOTE")))
                    {
                        fprintf(out, "<span class=\"code_note\">NOTE</span>");
                        i += sizeof("NOTE");
                    }
                    
                    while (i < contents.size && contents.data[i] != '\n') fputc(contents.data[i++], out);
                    fprintf(out, "</span>");
                }
                else if (i + 1 < contents.size && contents.data[i] == '/' && contents.data[i + 1] == '*')
                {
                    fprintf(out, "<span class=\"code_comment\">");
                    
                    umm comment_level = 0;
                    while (i < contents.size)
                    {
                        if (i + 1 < contents.size && contents.data[i] == '/' && contents.data[i] == '*')
                        {
                            comment_level += 1;
                            fputc(contents.data[i++], out);
                            fputc(contents.data[i++], out);
                        }
                        else if (i + 1 < contents.size && contents.data[i] == '*' && contents.data[i] == '/')
                        {
                            comment_level -= 1;
                            fputc(contents.data[i++], out);
                            fputc(contents.data[i++], out);
                            
                            if (comment_level == 0) break;
                        }
                        else fputc(contents.data[i++], out);
                    }
                    fprintf(out, "</span>");
                }
                else if (contents.data[i] == '\r') i += 1;
                else
                {
                    fputc(contents.data[i++], out);
                    
                    if (contents.data[i - 1] == '\n')
                    {
                        line_start = i;
                        line      += 1;
                    }
                }
            }
            
            if (!encountered_errors)
            {
                if (i == contents.size)
                {
                    //// ERROR:
                    fprintf(stderr, "ERROR(%llu:%llu): Missing ending ``` after code block\n", line, i - line_start + 1);
                    encountered_errors = true;
                }
                else
                {
                    i += 3;
                    fprintf(out, "</div>");
                }
            }
        }
        else if (contents.data[i] == '[')
        {
            i += 1;
            
            String display = { .data = contents.data + i, .size = 0 };
            while (i < contents.size && contents.data[i] != ']')
            {
                ++i;
                ++display.size;
            }
            
            if (i == contents.size)
            {
                //// ERROR:
                fprintf(stderr, "ERROR(%llu:%llu): Missing ending ] after link display text\n", line, i - line_start + 1);
                encountered_errors = true;
            }
            else
            {
                i += 1;
                
                if (i == contents.size || contents.data[i] != '(')
                {
                    //// ERROR:
                    fprintf(stderr, "ERROR(%llu:%llu): Missing ending address of link\n", line, i - line_start + 1);
                    encountered_errors = true;
                }
                else
                {
                    i += 1;
                    
                    String address = { .data = contents.data + i, .size = 0 };
                    while (i < contents.size && contents.data[i] != ')')
                    {
                        ++i;
                        ++address.size;
                    }
                    
                    if (i == contents.size)
                    {
                        //// ERROR:
                        fprintf(stderr, "ERROR(%llu:%llu): Missing ending ) after link address\n", line, i - line_start + 1);
                        encountered_errors = true;
                    }
                    else
                    {
                        i += 1;
                        
                        fprintf(out, "<a href=\"%.*s\">%.*s</a>", (int)address.size, address.data, (int)display.size, display.data);
                    }
                }
            }
        }
        else fputc(contents.data[i++], out);
    }
    
    if (!encountered_errors)
    {
        if (heading_level != 0) fprintf(out, "</h%llu>", heading_level);
        if (decor_level != 0)
        {
            if ((open_decors & 1) != 0) fprintf(out, "</span>");
            if ((open_decors & 2) != 0) fprintf(out, "</span>");
            if ((open_decors & 4) != 0) fprintf(out, "</span>");
            decor_level = 0;
            open_decors = 0;
        }
    }
    
    GeneratePostamble(out);
    
    return !encountered_errors;
}

bool
GenerateAllFiles(umm prefix_len, Growable_String path, String file_buffer, umm depth)
{
    bool encountered_errors = false;
    
    if (path.size + 3 > path.capacity)
    {
        //// ERROR: 
        fprintf(stderr, "Path too long. Ended at %.*s\n", (int)path.size, path.data);
        encountered_errors = true;
    }
    else
    {
        memcpy(path.data + path.size, "\\*", 3);
        path.size += 1;
        
        WIN32_FIND_DATAA find_data;
        HANDLE find_handle = FindFirstFileA((LPCSTR)path.data, &find_data);
        
        path.data[path.size] = 0;
        
        if (find_handle == INVALID_HANDLE_VALUE)
        {
            if (GetLastError() != ERROR_FILE_NOT_FOUND)
            {
                //// ERROR: 
                fprintf(stderr, "Failed to find files\n");
                encountered_errors = true;
            }
        }
        else
        {
            while (true)
            {
                umm filename_len = strlen(find_data.cFileName);
                
                if (path.size + filename_len + 1 > path.capacity)
                {
                    //// ERROR: 
                    fprintf(stderr, "Path too long. Ended at %.*s\n", (int)path.size, path.data);
                    encountered_errors = true;
                }
                else
                {
                    memcpy(path.data + path.size, find_data.cFileName, filename_len + 1);
                    path.size += filename_len;
                    
                    if (find_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0)
                        {
                            if (!CreateDirectoryA((LPCSTR)(path.data + prefix_len + 1), 0))
                            {
                                //// ERROR: 
                                fprintf(stderr, "Failed to create directory at %.*s\n",
                                        (int)(path.size - (prefix_len + 1)), path.data + prefix_len + 1);
                                encountered_errors = true;
                            }
                            else if (!GenerateAllFiles(prefix_len, path, file_buffer, depth + 1)) encountered_errors = true;
                        }
                    }
                    else if (find_data.dwFileAttributes == FILE_ATTRIBUTE_NORMAL ||
                             find_data.dwFileAttributes == FILE_ATTRIBUTE_ARCHIVE)
                    {
                        char* extension = 0;
                        for (imm i = path.size - 1; i >= 0; --i)
                        {
                            if (path.data[i] == '.')
                            {
                                extension = (char*)path.data + i;
                                break;
                            }
                        }
                        
                        if (extension == 0 || strcmp(extension, ".md") != 0)
                        {
                            if (!CopyFile((LPCSTR)path.data, (LPCSTR)(path.data + prefix_len + 1), true))
                            {
                                //// ERROR: 
                                fprintf(stderr, "Failed to copy file to %.*s\n",
                                        (int)(path.size - (prefix_len + 1)), path.data + prefix_len + 1);
                                encountered_errors = true;
                            }
                        }
                        else
                        {
                            if ((u8*)extension - path.data + sizeof(".html") > path.capacity)
                            {
                                //// ERROR: 
                                fprintf(stderr, "Path too long to attach .html extension at %.*s\n",
                                        (int)(path.size - (prefix_len + 1)), path.data + prefix_len + 1);
                                encountered_errors = true;
                            }
                            else
                            {
                                HANDLE read_file_handle = CreateFileA((LPCSTR)path.data, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
                                
                                DWORD bytes_read;
                                if (!ReadFile(read_file_handle, file_buffer.data, MIN((DWORD)file_buffer.size, find_data.nFileSizeLow), &bytes_read, 0) || bytes_read != find_data.nFileSizeLow)
                                {
                                    //// ERROR: 
                                    fprintf(stderr, "Failed to read file at %.*s\n", (int)path.size, path.data);
                                    encountered_errors = true;
                                }
                                
                                CloseHandle(read_file_handle);
                                
                                if (!encountered_errors)
                                {
                                    memcpy(extension, ".html", sizeof(".html"));
                                    filename_len += (sizeof(".html") - sizeof(".md"));
                                    path.size    += (sizeof(".html") - sizeof(".md"));
                                    
                                    FILE* gen_file = 0;
                                    
                                    HANDLE gen_file_handle = CreateFileA((LPCSTR)(path.data + prefix_len + 1), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
                                    
                                    if (gen_file_handle != INVALID_HANDLE_VALUE)
                                    {
                                        int gen_file_descriptor = _open_osfhandle((intptr_t)gen_file_handle, _O_WTEXT);
                                        
                                        if (gen_file_descriptor != -1)
                                        {
                                            gen_file = _fdopen(gen_file_descriptor, "w");
                                            
                                            if (!gen_file) _close(gen_file_descriptor);
                                        }
                                        else CloseHandle(gen_file_handle);
                                    }
                                    
                                    if (gen_file == 0)
                                    {
                                        //// ERROR: 
                                        fprintf(stderr, "Failed to open file at %.*s\n",
                                                (int)(path.size - (prefix_len + 1)), path.data + prefix_len + 1);
                                        encountered_errors = true;
                                    }
                                    else
                                    {
                                        String filename_wo_ext = {
                                            .data = path.data + (path.size - filename_len),
                                            .size = filename_len - sizeof(".html")
                                        };
                                        
                                        bool is_in_devlogs = String_Match((String){ .data = path.data, .size = path.size - filename_len },
                                                                          STRING("..\\pages\\devlogs\\"));
                                        
                                        if (!GenerateFile((String){ .data = file_buffer.data, .size = bytes_read}, filename_wo_ext, gen_file, depth, is_in_devlogs))
                                        {
                                            encountered_errors = true;
                                        }
                                        
                                        fclose(gen_file);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        //// ERROR: 
                        printf("Unknown file type! %s %d\n", find_data.cFileName, find_data.dwFileAttributes);
                        encountered_errors = true;
                    }
                    
                    path.size -= filename_len;
                    path.data[path.size] = 0;
                }
                
                if      (encountered_errors)                     break;
                else if (FindNextFileA(find_handle, &find_data)) continue;
                else
                {
                    if (GetLastError() != ERROR_NO_MORE_FILES)
                    {
                        //// ERROR: 
                        fprintf(stderr, "Failed to find all files\n");
                        encountered_errors = true;
                    }
                    
                    break;
                }
            }
        }
        
        FindClose(find_handle);
    }
    
    return !encountered_errors;
}

void
main()
{
    char* command_line = GetCommandLineA();
    
    char* filename = command_line;
    char* wd_path  = (*command_line == '"' ? command_line + 1 : command_line);
    for (char* scan = wd_path; *scan != 0; ++scan)
    {
        if (*scan == '\\' || *scan == '//') filename = scan + 1;
        else if (*command_line != '"' && *scan == ' ') break;
        else if (*command_line == '"' && *scan == '"') break;
    }
    
    *filename = 0;
    
    if (!SetCurrentDirectory(wd_path) || !SetCurrentDirectory("..\\gen")) fprintf(stderr, "Failed to set current directory\n");
    else
    {
        HANDLE change_handle = FindFirstChangeNotificationA("..\\pages", true, (FILE_NOTIFY_CHANGE_FILE_NAME  |
                                                                                FILE_NOTIFY_CHANGE_DIR_NAME   |
                                                                                FILE_NOTIFY_CHANGE_ATTRIBUTES |
                                                                                FILE_NOTIFY_CHANGE_SIZE       |
                                                                                FILE_NOTIFY_CHANGE_LAST_WRITE));
        system("rmdir . /s /q 2>nul");
        
        String pages_path = STRING("..\\pages\0");
        
        char buffer[MAX_PATH * 4];
        memcpy(buffer, pages_path.data, pages_path.size);
        
        Growable_String path = {
            .data     = (u8*)buffer,
            .size     = pages_path.size - 1,
            .capacity = ARRAY_SIZE(buffer),
        };
        
        String file_buffer;
        file_buffer.size = GB(2);
        file_buffer.data = VirtualAlloc(0, file_buffer.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        
        if (file_buffer.data == 0) fprintf(stderr, "Failed to create file buffer\n");
        else
        {
            LogIndex = fopen("log_index.html", "w");
            
            if (LogIndex == 0) fprintf(stderr, "Failed open log index\n");
            else
            {
                GeneratePreamble(LogIndex, STRING("Devlogs"), 0);
                fprintf(LogIndex, "<h1>Devlogs</h1>");
                
                GenerateAllFiles(pages_path.size - 1, path, file_buffer, 0);
                
                GeneratePostamble(LogIndex);
                fclose(LogIndex);
            }
        }
    }
}
