clear all; close all; clc

homedir = 'C:\Users\NVL Laptop\Desktop\ARL_Codes\readstresstimehistorydata_c\';
nodes = [];
n1 = 1;
n2 = 2000;
for ii = 1:8
    tmp = n1:n2;
    n1 = n2+1;
    n2 = n1+1999;
    nodes = vertcat(nodes,tmp);
end

for ii = 12:21
    cd(homedir)
    mkdir(sprintf('run%i',ii));
    cd(strcat(homedir,sprintf('run%i',ii)))
    for jj = 0:19
        mkdir(sprintf('FEA.%i',jj));
        curdir = strcat(homedir,sprintf('run%i',ii),filesep,sprintf('FEA.%i',jj),filesep);
        cd(curdir)
        for kk = 1:8
            fid = fopen(strcat(curdir,sprintf('file_%i_%i_%i',ii,jj,kk),'.txt'),'w+');
            fprintf(fid,'header 1\nheader 2\n');
            data = randn(4,size(nodes,2));
            fprintf(fid,'%8i,%15.8f,%15.8f,%15.8f,%15.8f\n',[nodes(kk,:);data]);
            fprintf(fid,'*ELEMENT\njunk\njunk');
            fclose(fid);
        end
        cd ..
    end
end
cd(homedir)
