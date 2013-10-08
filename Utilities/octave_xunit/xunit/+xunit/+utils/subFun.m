function subFunList = subFun(mFile)
%SUBFUN lists all subfunction names in specified MFILE.

%   Arnaud Barré
%   Copyright 2013, xUnit for Octave

% This function and required subfunctions are largely inspired by the code of the function SubFuns (http://www.mathworks.com/matlabcentral/fileexchange/5563)

error(nargchk(1,1,nargin))
error(nargoutchk(1,1,nargout))
%make sure m-file exists (on path)
if ~exist(mFile)
  error(['Unable to find m-file ' mFile])
end
%open m-file (read access)
fid = fopen(which(mFile));
%find all lines starting with 'function' signature
subFunList = {};
lineNum = 0;
while ~feof(fid) %check one line at a time
  lineNum = lineNum+1; %advance line pointer
  try %read line
    txt = fgetl(fid);
  catch %file read error
    disp(['Tried reading file ' mFile])
    error(sprintf('Unable to read line %d',lineNum))
  end
  name = functionName(txt);
  if ~isempty(name) %found one, add to list
    subFunList(end+1) = name;
  end
end
%close file when done
fclose(fid);
if (length(subFunList) > 1)
  subFunList = subFunList(2:end);
else
  subFunList = {};
end
end % subFun

function name = functionName(txt)
%FUNCTIONNAME returns name of subfunction.
%   Returns empty if line does not contain valid function name.
%
%Usage:
%   name = functionName(txt)

%TODO List:
%   1. double check that algorithm worked correctly

%remove leading blanks
txt = noLeadBlanks(txt);
if length(txt)<8 %line too short
  name = []; %return empty (no valid function here)
  return;
end
if strcmp('function',txt(1:8)) %function line
  %strip off function keyword at beginning
  txt(1:8) = [];
  %ignore outputs
  if strfind(txt,'=') %outputs defined
    loc = strfind(txt,'=');     %LHS before =
    txt(1:loc) = [];            %ignore outputs
  end
  %remove leading blanks before name
  txt = noLeadBlanks(txt);
  %ignore EOL comments
  if strfind(txt,'%')
    loc = strfind(txt,'%');     %comment from % on
    txt(loc:end) = [];          %ignore inputs
  end
  %ignore inputs
  if strfind(txt,'(')
    loc = strfind(txt,'(');     %inputs after (
    txt(loc:end) = [];          %ignore inputs
  end
  %remove trailing blanks after function name
  while txt(end)==' '
    txt(end) = [];
  end
  %for legal function names, only thing left must be function name
  name  = txt;
else %not function line
  name = []; %return empty
end
end %functionName (subfunction)

function txt = noLeadBlanks(txt) 
%NOLEADBLANK removes leading spaces.
  done = false;
  while ~done
    if isempty(txt)
      return
    elseif txt(1)==' ' %leading blank
      txt(1) = []; %remove
    else %done (nonblank first character)
      done = true;
    end
  end %while (done or not)
end %noLeadBlanks (subfunction)