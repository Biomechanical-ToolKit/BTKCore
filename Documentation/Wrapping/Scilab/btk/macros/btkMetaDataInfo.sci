function info = btkMetaDataInfo(format,values)
// Ouput variables initialisation (not found in input variables)
info=[];
// Display mode
mode(0);
// Display warning for floating point exception
ieee(1);

info.format = format;
info.values = values;
endfunction
