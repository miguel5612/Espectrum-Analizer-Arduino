   
function varargout = Main(varargin)
% MAIN MATLAB code for Main.fig
%      MAIN, by itself, creates a new MAIN or raises the existing
%      singleton*.
%
%      H = MAIN returns the handle to a new MAIN or the handle to
%      the existing singleton*.
%
%      MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAIN.M with the given input arguments.
%
%      MAIN('Property','Value',...) creates a new MAIN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before Main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to Main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help Main

% Last Modified by GUIDE v2.5 17-Jun-2018 16:09:33

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @Main_OpeningFcn, ...
                   'gui_OutputFcn',  @Main_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT

% --- Executes just before Main is made visible.
function Main_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to Main (see VARARGIN)

% Choose default command line output for Main
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% This sets up the initial plot - only do when we are invisible
% so window can get raised using Main.
if strcmp(get(hObject,'Visible'),'off')
    %plot(1);
end
%Default load program
global s
global pauseStatus
g= imread('main.jpg');
axes(handles.axes2)
imshow(g);
pauseStatus = 0;
set(handles.slider1,'Enable','off')
set(handles.slider2,'Enable','off')
f = 420:0.1:460
y = zeros(401,1)
plot(f,y,'r','parent',handles.axes1);
set(handles.axes1,'color','g')
grid(handles.axes1,'minor')

%set(findobj(gcf, 'type','axes1'), 'Visible','off');
% Miguel.califa --> Se automatiza el inicio de los puertos com
%Aqui se declara la funcion que se va a ejecutar cada segundo
% UIWAIT makes Main wait for user response (see UIRESUME)
% uiwait(handles.figure1);
    

% --- Outputs from this function are returned to the command line.
function varargout = Main_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
% --- Executes on button press in pushbutton1.
function pushbutton1_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clc;
delete(instrfindall);
    popup_sel_index = get(handles.popupmenu1, 'Value');
    switch popup_sel_index
    case 1
        s = serial('COM1','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 2
        s = serial('COM2','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 3
        s = serial('COM3','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 4
        s = serial('COM4','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 5
        s = serial('COM5','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 6
        s = serial('COM6','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 7
        s = serial('COM7','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            delete(instrfindall);
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            fopen(s);
        end
    case 8
        s = serial('COM8','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 9
        s = serial('COM9','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 10
        s = serial('COM10','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 11
        s = serial('COM11','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 12
        s = serial('COM12','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 13
        s = serial('COM13','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 14
        s = serial('COM14','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end
    case 15
        s = serial('COM15','BAUD',115200);
        s.InputBufferSize = 2048;
        try
            fopen(s);
        catch
            msgbox('No se puede iniciar el puerto serie');
            fclose(s);
            delete(instrfindall);
            fopen(s);
        end

    end
set(findobj(gcf, 'type','axes1'), 'Visible','true')
s.BytesAvailableFcn = {@showDataSerial,hObject}

function showDataSerial(s, eventdata, handles)
handles = guidata(handles);
h = handles.axes1;
if(get(handles.edit2,'String') == '0')
    fgetl(s)
    data1 = fgetl(s)
    splittedPoints1 = strsplit(data1,',')
    numbers1 = str2double(splittedPoints1)
    [m,n] = size(splittedPoints1)
    mychar = splittedPoints1(n)
    StrEnd = mychar{1}
    FrecuenciaInferior = numbers1(2)
    FrecuenciaSuperior = numbers1(3)
    Paso = numbers1(4)
    if strfind(StrEnd,'END')
        numbers3 = [numbers1(5:n)];
        f = FrecuenciaInferior:Paso:FrecuenciaSuperior
        [o,p] = size(numbers3)
        [q,r] = size(f);
        limit = get(handles.slider3,'Value');
        mensaje = strcat('UMBRAL DE MEDICION (',num2str(limit),')');
        set(handles.text11,'String',mensaje);
        if p==r
            for i=1:p
                numbers3(i);
                if numbers3(i)<=limit
                    numbers3(i) = 0; %En caso de no cumplir se atenua (Se asume es ruido)
                end                
            end
            FigPrinted = stem(f,numbers3,'r','parent',h);
        else
            numbers3 = [numbers1(6:p)];
            [a,b] = size(numbers3)
            [c,d] = size(f)
            diff = b - d 
            numbers3 = [numbers1(1:b-diff)];
            [a,b] = size(numbers3)
            for i=1:b-1
                if numbers3(i)<=limit
                    numbers3(i) = 0; %En caso de no cumplir se atenua (Se asume es ruido)
                end                
            end    
            [a,b] = size(numbers3)
            [c,d] = size(f)
            FigPrinted = stem(f,numbers3,'r','parent',h);
        end
        Fc = (FrecuenciaInferior+((FrecuenciaSuperior-FrecuenciaInferior)/2));
        title('Signal Analysis','parent',h);
        ylabel('Power received RSSi (dBm)','parent',h);
        xlabel('Frecuency (MHz)','parent',h);
        grid(h,'minor');
        set(h,'color','g')
        pause(0.1);
        
        try
        set(handles.slider1,'Enable','on');
        set(handles.slider2,'Enable','on');
        set(handles.slider2,'Value',FrecuenciaInferior);
        set(handles.slider1,'Value',FrecuenciaSuperior);
        set(handles.slider1,'Enable','on');
        set(handles.slider2,'Enable','on');
        set(handles.pushbutton1,'String','RECONECTAR');
        end
        FrecuenciaSuperior = numbers1(2);
        FrecuenciaInferior = numbers1(3);
        set(handles.slider2,'Value',FrecuenciaInferior);
        set(handles.slider1,'Value',FrecuenciaSuperior);
        Mensaje = strcat('F: ' ,num2str(FrecuenciaSuperior),'-',num2str(FrecuenciaInferior),' (MHz)');
        Mensaje2 =strcat('Fc: ' ,num2str(Fc),' (MHz)');
        set(handles.text3,'String',Mensaje);
        set(handles.text7,'String',Mensaje2);
    end
 
    
end
  
    


% --------------------------------------------------------------------
function FileMenu_Callback(hObject, eventdata, handles)
% hObject    handle to FileMenu (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --------------------------------------------------------------------
function OpenMenuItem_Callback(hObject, eventdata, handles)
% hObject    handle to OpenMenuItem (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
file = uigetfile('*.fig');
if ~isequal(file, 0)
    open(file);
end

% --------------------------------------------------------------------
function PrintMenuItem_Callback(hObject, eventdata, handles)
% hObject    handle to PrintMenuItem (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
printdlg(handles.figure1)

% --------------------------------------------------------------------
function CloseMenuItem_Callback(hObject, eventdata, handles)
% hObject    handle to CloseMenuItem (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
selection = questdlg(['Close ' get(handles.figure1,'Name') '?'],...
                     ['Close ' get(handles.figure1,'Name') '...'],...
                     'Yes','No','Yes');
if strcmp(selection,'No')
    return;
end

delete(handles.figure1)


% --- Executes on selection change in popupmenu1.
function popupmenu1_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns popupmenu1 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu1


% --- Executes during object creation, after setting all properties.
function popupmenu1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
     set(hObject,'BackgroundColor','white');
end
set(hObject, 'String', {'COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7', 'COM8', 'COM9', 'COM10', 'COM11', 'COM12', 'COM13', 'COM14', 'COM15'});


% --- Executes on button press in pushbutton3.
function pushbutton3_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on slider movement.
function slider1_Callback(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function slider1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on slider movement.
function slider2_Callback(hObject, eventdata, handles)
% hObject    handle to slider2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function slider2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end

%Esta es la funcion que se va a ejecutar cada segundo


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over popupmenu1.

% hObject    handle to popupmenu1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on key press with focus on popupmenu1 and none of its controls.
% hObject    handle to popupmenu1 (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on slider movement.
function slider3_Callback(hObject, eventdata, handles)
% hObject    handle to slider3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider


% --- Executes during object creation, after setting all properties.
function slider3_CreateFcn(hObject, eventdata, handles)
% hObject    handle to slider3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in pushbutton4.
function pushbutton4_Callback(hObject, eventdata, handles)
% hObject    handle to pushbutton4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if get(handles.edit2,'String') == '0'
    set(handles.edit2,'String','1');
    set(handles.pushbutton4,'String','CONTINUAR');
else
    set(handles.edit2,'String','0');
    set(handles.pushbutton4,'String','CONTINUAR');
end



function edit2_Callback(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit2 as 

%        str2double(get(hObject,'String')) returns contents of edit2 as a double


% --- Executes during object creation, after setting all properties.
function edit2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
