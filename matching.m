l = rgb2gray(imread('left.png'));
r = rgb2gray(imread('right.png'));

h = size(l);
msize = 3;
offset = 1;
disparity = zeros(size(l)-[2*offset,2*offset]);

for i = 1+offset:h(1)-offset
    L = zeros(h(2)-2*offset,msize*msize);
    R = zeros(h(2)-2*offset,msize*msize);
    for j = 1+offset:h(2)-offset
        currentL = l(i-offset:i+offset,j-offset:j+offset);
        currentL = double(reshape(currentL,1,[]));
        currentR = r(i-offset:i+offset,j-offset:j+offset);
        currentR = double(reshape(currentR,1,[]));
        currentR = (currentR-mean(currentR))/norm(currentR);
        currentL = (currentL-mean(currentL))/norm(currentL);
        R(j,:) = currentR;
        L(j,:) = currentL;
    end
    L = double(L);
    R = double(R);
    distances = pdist2(L,R,@NCC);
    rowdisp = zeros(1,h(2)-2);
    for j = 1+offset:h(2)-offset
        [C, I] = min(distances(j,:));
        disparity(i,j) = abs(I-j);
    end
end
gt = cell2mat(struct2cell(load('gt.mat')));
gt = gt(offset:size(disparity,1),offset:size(disparity,2));
error = eval_disp(disparity, gt)