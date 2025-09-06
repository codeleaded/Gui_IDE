#include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
#include "C:/Wichtig/System/Static/Library/ObjectNotation.h"

int OffsetX = 100;
int OffsetY = 100;
int Size = 32;
int PathSize = 32;

char* FilePath = NULL;

Font OutPath;

String HighLighter;

TextBox Field;
TextBox SavingPath;

Scene scene;

void ChooseFile(){
    OPENFILENAME ofn;
    char FileBuff[260] = { 0 };
    HWND hwnd;
    HANDLE hf;

    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = FileBuff;
    ofn.nMaxFile = sizeof(FileBuff);
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(GetOpenFileName(&ofn)){
        //printf("File: %s\n",ofn.lpstrFile);
        Input_SetText(&SavingPath.In,ofn.lpstrFile);
    }else{
        //printf("None Selected.\n");
    }
}
void ChooseFileHigh(){
    OPENFILENAME ofn;
    char FileBuff[260] = { 0 };
    HWND hwnd;
    HANDLE hf;

    ZeroMemory(&ofn,sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = FileBuff;
    ofn.nMaxFile = sizeof(FileBuff);
    ofn.lpstrFilter = "All Files\0*.*\0ALXON Files\0*.alxon\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = "C:\\Wichtig\\System\\SyntaxFiles\\";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(GetOpenFileName(&ofn)){
        //printf("File: %s\n",ofn.lpstrFile);
        String_Clear(&HighLighter);
        String_Append(&HighLighter,ofn.lpstrFile);
    }else{
        //printf("None Selected.\n");
    }
}

void Button_Save(ButtonEvent* be,Button* b){
    if(be->e.EventType==EVENT_PRESSED){
        char* Path = String_CStr(&SavingPath.In.Buffer);
        char* Data = String_CStr(&Field.In.Buffer);

        FILE* f = fopen(Path,"w");
        if(f){
            fprintf(f,"%s",Data);
            fclose(f);
        }

        free(Path);
        free(Data);
    }
}
void Button_Load(ButtonEvent* be,Button* b){
    if(be->e.EventType==EVENT_PRESSED){
        char* Path = String_CStr(&SavingPath.In.Buffer);
        
        FILE* f = fopen(Path,"r");
        if(f){
            fseek(f,0,SEEK_END);
            int Length = ftell(f);
            fseek(f,0,SEEK_SET);

            char* BufferIn = (char*)malloc(Length+1);
            fread(BufferIn,1,Length,f);
            BufferIn[Length] = '\0';

            String_Clear(&Field.In.Buffer);
            String_Append(&Field.In.Buffer,BufferIn);
            free(BufferIn);

            Field.In.Enabled = FALSE;
            Field.In.LastKey = 0;
            Field.In.LastChar = 0;
            Field.In.CharBefore = 0;
            Field.In.KeyBefore = 0;
            Field.In.Curser = 0;
            Field.In.CurserEnd = -1;
            Field.In.Again = FALSE;
            Field.In.FirstPress = 0;
            Field.In.LastTime = 0;

            Field.ScrollX = 0;
            Field.ScrollY = 0;

            fclose(f);
        }
        free(Path);
    }
}
void Button_Search(ButtonEvent* be,Button* b){
    if(be->e.EventType==EVENT_PRESSED){
        ChooseFile();
    }
}
void Button_SearchHigh(ButtonEvent* be,Button* b){
    if(be->e.EventType==EVENT_PRESSED){
        ChooseFileHigh();

        char* cstr = String_CStr(&HighLighter);
        TextBox_SetSyntax(&Field,cstr);
        free(cstr);
    }
}

void Setup(AlxWindow* w){
    HighLighter = String_New();
    if(!FilePath) FilePath = CStr_Cpy("C:/Wichtig/Hecke/C/Win_IDE/Saved.c");
    
    SetFont(FONT_YANIS);
    ResizeFont(Size,Size);

    OutPath = Font_New(FONT_YANIS);
    Font_Resize(&OutPath,PathSize,PathSize);

    SavingPath = TextBox_New(Input_New(INPUT_MAXLENGTH,1),(Rect){ { OffsetX * 4.1f,OffsetY*0.5f-Size*0.5f },{ GetWidth()-OffsetX * 3.1f,OffsetY*0.5f-Size*0.5f } },FONT_PATHS_YANIS,Size,Size,WHITE);
    Input_SetText(&SavingPath.In,FilePath);
    TextBox_SetSyntax(&SavingPath,"C:/Wichtig/System/SyntaxFiles/Path_Syntax.alxon");

    Field = TextBox_New(Input_New(INPUT_MAXLENGTH,INPUT_MAXLENGTH),(Rect){ { OffsetX,OffsetY },{ GetWidth()-OffsetX,GetHeight()-OffsetY } },FONT_PATHS_YANIS,Size,Size,WHITE);
    TextBox_SetSyntax(&Field,"C:/Wichtig/System/SyntaxFiles/C_Syntax.alxon");

    scene = Scene_New();
    Scene_Add(&scene,(Button[]){ Button_NewStd("Save",Button_Save,(Vec2){19.0f,19.0f},(Rect){ {0.0f,0.0f},{OffsetX,OffsetY} },RED,BLACK) },sizeof(Button));
    Scene_Add(&scene,(Button[]){ Button_NewStd("Load",Button_Load,(Vec2){19.0f,19.0f},(Rect){ {OffsetX,0.0f},{OffsetX,OffsetY} },GREEN,BLACK) },sizeof(Button));
    Scene_Add(&scene,(Button[]){ Button_NewStd("Find",Button_Search,(Vec2){19.0f,19.0f},(Rect){ {OffsetX*2,0.0f},{OffsetX,OffsetY} },BLUE,BLACK) },sizeof(Button));
    Scene_Add(&scene,(Button[]){ Button_NewStd("Color",Button_SearchHigh,(Vec2){19.0f,19.0f},(Rect){ {OffsetX*3,0.0f},{OffsetX,OffsetY} },YELLOW,BLACK) },sizeof(Button));
}

void Update(AlxWindow* w){
    if(Field.In.Again && Stroke(VK_OEM_PLUS).DOWN && Stroke(VK_CONTROL).DOWN){
        Size += 3;
        TextBox_FontSesize(&Field,Size,Size);
    }else if(Field.In.Again && Stroke(VK_OEM_MINUS).DOWN && Stroke(VK_CONTROL).DOWN){
        Size -= 3;
        TextBox_FontSesize(&Field,Size,Size);
    }else if(Field.In.Again && Stroke(VK_UP).DOWN && Stroke(VK_CONTROL).DOWN){
        Field.ScrollY += 1;
        if(Field.ScrollY>0) Field.ScrollY = 0;
    }else if(Field.In.Again && Stroke(VK_DOWN).DOWN && Stroke(VK_CONTROL).DOWN){
        Field.ScrollY -= 1;
        char* cstr = String_CStr(&Field.In.Buffer);
        int Max = (CStr_CountOf(cstr,'\n')+4) - ((GetHeight()-OffsetY) / (Field.font.CharSizeY * INPUT_GAP_FAKTOR));
        free(cstr);
        if(Field.ScrollY<-Max)  Field.ScrollY = -Max;
        if(Field.ScrollY>0)     Field.ScrollY = 0;
    }else{
        Input_DefaultReact(&Field.In);
    }

    Scene_Update(&scene,window.Strokes,(Vec2){GetMouse().x,GetMouse().y});
    
    Input_DefaultReact(&SavingPath.In);
    TextBox_Update(&SavingPath,GetMouse());
    TextBox_Update(&Field,GetMouse());

    Clear(BLACK);

    RenderRect(0.0f,0.0f,OffsetX,GetHeight(),GRAY);

    TextBox_Render(WINDOW_STD_ARGS,&Field);

    int Chop = 0;
    int Lines = 0;
    int Chars = 0;
    int CharsCurser = 0;
    int LinesCurser = 0;

    if(Field.ScrollY!=0){
        for(int i = Chop;i<Field.In.Buffer.str.size;i++){
            char c = String_Get(&Field.In.Buffer,i);
            if(c=='\n' || i==Field.In.Buffer.str.size-1){
                Chars = i - Chop;
                if(Field.In.Curser>=Chop && Field.In.Curser<=i+1){
                    CharsCurser = Field.In.Curser-Chop;
                    LinesCurser = Lines;
                }
                Chop = i+1;
                Lines++;
                if(Lines>=-Field.ScrollY) break;
            }
            if(c=='\n' && i==Field.In.Buffer.str.size-1){
                CharsCurser = 0;
                LinesCurser = Lines;
                Lines++;
                Chars = -1;
            }
        }
    }
    for(int i = Chop;i<Field.In.Buffer.str.size;i++){
        char c = String_Get(&Field.In.Buffer,i);
        if(c=='\n' || i==Field.In.Buffer.str.size-1){
            char Buff[20];
            sprintf(Buff,"%d",Lines);
            RenderCStrFont(&Field.font,Buff,0,OffsetY + (Field.ScrollY + Lines) * (Field.font.CharSizeY * INPUT_GAP_FAKTOR),BLACK);

            Chars = i - Chop;
            if(Field.In.Curser>=Chop && Field.In.Curser<=i+1){
                CharsCurser = Field.In.Curser-Chop;
                LinesCurser = Lines;
            }
            Chop = i+1;
            Lines++;
        }
        if(c=='\n' && i==Field.In.Buffer.str.size-1){
            CharsCurser = 0;
            LinesCurser = Lines;
            Lines++;
            Chars = -1;
        }
    }

    RenderRect(0.0f,0.0f,GetWidth(),OffsetY,DARK_GRAY);

    Scene_Render(WINDOW_STD_ARGS,&scene);

    TextBox_Render(WINDOW_STD_ARGS,&SavingPath);
}

void Delete(AlxWindow* w){
    String_Free(&HighLighter);
    Font_Free(&OutPath);

    TextBox_Free(&SavingPath);
    TextBox_Free(&Field);
    free(FilePath);
    Scene_Free(&scene);
}

int main(int argc,char** argv){
    if(argc>1){
        FilePath = CStr_Cpy(argv[1]);
    }
    
    if(Create("Game Test",1900,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}