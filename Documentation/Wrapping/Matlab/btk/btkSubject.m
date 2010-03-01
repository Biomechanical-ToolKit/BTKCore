function subject = btkSubject(sex, height, weight)
%BTKSUBJECT Create a structure containing data related to a subject.
%
%  SUBJECT = BTKSUBJECT(SEX, HEIGHT, WEIGHT) creates the structure SUBJECT which embed SEX, HEIGHT and WEIGHT 
%  in the field 'sex', 'height' and 'weight' respectively.
%
%  The SUBJECT INFO must be set with the following values' format:
%   - SUBJECT.SEX: contains a string ('F' or 'Female' for the female sex and 'M' or 'Male' for the male sex).
%   - SUBJECT.HEIGHT: Subject's heigt expressed in the same unit than markers' unit (generally in millimeters).
%   - SUBJECT.WEIGHT: Subject's weight in kilograms'.

%  Author: A. Barré
%  Copyright 2009-2010 Biomechanical ToolKit (BTK).

subject.sex = sex;
subject.height = height;
subject.weight = weight;
