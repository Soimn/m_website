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

bool
GenerateFile(String contents, String filename, FILE* out)
{
    bool encountered_errors = false;
    
    umm line_start = 0;
    umm line       = 1;
    
    String title = filename;
    
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
                        while (i < contents.size && (contents.data[i] >= 'a' && contents.data[i] <= 'z' ||
                                                     contents.data[i] >= 'A' && contents.data[i] <= 'Z' ||
                                                     contents.data[i] >= '0' && contents.data[i] <= '9' ||
                                                     contents.data[i] == '_'))
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
                else
                {
                    //// ERROR:
                    fprintf(stderr, "ERROR(%llu:%llu): Unknown config argument\n", line, i - line_start);
                    encountered_errors = true;
                }
            }
        }
        
        if (i == contents.size || contents.data[i] != '$')
        {
            //// ERROR:
            fprintf(stderr, "ERROR(%llu:%llu): Missing $ after configuration\n", line, i - line_start);
            encountered_errors = true;
        }
        else i += 1;
    }
    
    fprintf(out, "<!doctype html>\n<html>\n<head>\n");
    fprintf(out, "<title>%.*s</title>\n", (int)title.size, title.data);
    fprintf(out, "<link rel=\"stylesheet\" href=\"main.css\">\n");
    fprintf(out, "</head>\n<body>\n");
    fprintf(out, "<div class=\"content\">\n");
    
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
                heading_level = j - i;
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
                    
                    if (k != ARRAY_SIZE(keywords)) fprintf(out, "<span class=\"code_keyword\">");
                    fprintf(out, "%.*s", (int)identifier.size, identifier.data);
                    if (k != ARRAY_SIZE(keywords)) fprintf(out, "</span>");
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
                    fprintf(out, "<span class=\"code_comment\">");
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
    
    if (!encountered_errors)
    {
        fprintf(out, "</div>\n</body>\n</html>");
    }
    
    return !encountered_errors;
}

bool
GenerateAllFiles(umm prefix_len, Growable_String path, String file_buffer)
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
                            else if (!GenerateAllFiles(prefix_len, path, file_buffer)) encountered_errors = true;
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
                        
                        if (extension == 0 || strcmp(extension, ".md") != 0) printf("Skipping %.*s\n", (int)path.size, path.data);
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
                                        
                                        if (!GenerateFile((String){ .data = file_buffer.data, .size = bytes_read}, filename_wo_ext, gen_file))
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
            GenerateAllFiles(pages_path.size - 1, path, file_buffer);
            CopyFile("..\\src\\main.css", ".\\main.css", 0);
        }
    }
}
