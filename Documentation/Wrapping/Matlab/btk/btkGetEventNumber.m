function num = btkGetEventNumber(h)
%BTKGETEVENTNUMBER Return the number of events.
%
%  NUM = BTKGETEVENTNUMBER(H) returns the number of events associated with the 
%  acquisitions reprenseted by the handle. This handle is obtained by the use of
%  a btk* function.

%  Author: A. Barré
%  Copyright 2009-2013 Biomechanical ToolKit (BTK).

num = length(btkGetEventsValues(h));
