clear all
close all
clc

A = textread('sphere_veryhigh.txt', '%s');
B = strrep(A, 'D', 'e');

C = str2double(B);

len = length(C);

node_index = find(C==2411,10); %expand the second number for larger matrixes

triangle_index = find(C==2412,10); %expand the second number for larger matrixes

lastfield_index = find(C==2420,10); %expand the second number for larger matrixes

%this block of code goes through each instance of 2412 found, and checks if
%the value before it is -1. If so, then the 2412 signifies the start of the
%triangles field. Note that this does not check the first instance of 2412,
%so there must be more than 2412 triangles

r = 1; c= 1; vals_taken = 0;
start_of_triangle_field = 0;

for (i = 1:length(node_index))
    if C(node_index(i)-1) == -1
        start_of_node_field = node_index(i) + 5; 
    end
end

for (i = 1:length(triangle_index))
    if C(triangle_index(i)-1) == -1
        start_of_triangle_field = triangle_index(i) + 1; 
    end
end

for (i = 1:length(lastfield_index))
    if C(lastfield_index(i)-1) == -1
        end_of_triangle_field = lastfield_index(i) - 2; 
    end
end

%if C(index(1)-1) == -1 %this is for small meshes, just in case less than 2412
 %   start_of_triangle_field = index(1) + 1; 
%end   

%nodes = zeros(numnodes,3)

i = start_of_node_field; % start i at 7
while (i < start_of_triangle_field - 2) % build node matrix
    
    if c == 4
        c = 1;
        r = r + 1;
    end
    nodes(r,c) = C(i); %change between C and D, C currently will not show
    c = c + 1;
    vals_taken = vals_taken + 1;
    i = i + 1;
    if vals_taken == 3
       i = i + 4; % skip ahead by 4, taken 3 values (vertexes) already. Don't care about the next 3 or 4?? entries
       vals_taken = 0;
    end
end

r = 1; c = 1; vals_taken = 0;
i = start_of_triangle_field + 6 % this is the first node in the triangle field
while (i < end_of_triangle_field) % build the triangle matrix
    
    if c == 4
        c = 1;
        r = r + 1;
    end
    
    triangles(r,c) = C(i);
    
    c = c + 1;
    vals_taken = vals_taken + 1;
    i = i + 1;
    if vals_taken == 3
       i = i + 6; % skip ahead by 7, taken 3 values (nodes) already. Don't care about the next 6 entries
       vals_taken = 0;
    end
end
x = nodes(:,1);
y = nodes(:, 2);
z = nodes(:, 3);
trimesh (triangles, x, y, z);
hold on;

for (i = 1:length(triangles))
    nodeA = triangles(i,1);
    nodeB = triangles(i,2);
    nodeC = triangles(i,3);
    pts_A(i,:) = nodes(nodeA,:);
    pts_B(i,:) = nodes(nodeB,:);
    pts_C(i,:) = nodes(nodeC,:);
    vect1(i,:) = (nodes(nodeA,:) - nodes(nodeB,:));
    nodeA = triangles(i,1);
    vect2(i,:) =  nodes(nodeA,:) - nodes(nodeC,:);
    
    
end

%calculate a matrix of centroids, in order to fit the normal vectors to
triangle_coords_x = horzcat(pts_A(:,1), pts_B(:,1), pts_C(:,1)); %cocatenate the first column (x coords) of each node matrix
triangle_coords_y = horzcat(pts_A(:,2), pts_B(:,2), pts_C(:,2)); %cocatenate the second column (y coords) of each node matrix
triangle_coords_z = horzcat(pts_A(:,3), pts_B(:,3), pts_C(:,3)); %cocatenate the third column (z coords) of each node matrix
centroids = horzcat(mean(triangle_coords_x, 2), mean(triangle_coords_y, 2), mean(triangle_coords_z, 2));

normals = cross(vect1,vect2);
normals =  normr(normals);
x = normals(:,1);
y = normals(:, 2);
z = normals(:, 3);

u = centroids(:,1);
v = centroids(:, 2);
q = centroids(:, 3);

quiver3( u, v, q, x,y,z, 2);
axis equal;
xlabel('X');
ylabel('Y');
zlabel('Z');

