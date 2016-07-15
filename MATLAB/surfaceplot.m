clear all
close all
clc

A = textread('shape.txt', '%s');
B = strrep(A, 'D', 'e');

C = str2double(B);

len = length(C);
 %expand the second number for larger matrixes; 10 may not be sufficient
 %find returns vector of all matches. Result may be a vector if  multiple
node_index = find(C==2411,10); %search file to find the marker '2411' indicating begining of node field
triangle_index = find(C==2412,10); %search file to find the marker '2412' indicating begining of triangle field
lastfield_index = find(C==2420,10); %search file to find the marker '2420' indicating end of triangle field

%this block of code goes through each instance of 2412 found, and checks if
%the value before it is -1. If so, then the 2412 signifies the start of the
%triangles field. Note that this does not check the first instance of 2412,
%so there must be more than 2412 triangles

r = 1; c= 1; vals_taken = 0;
start_of_triangle_field = 0;

for i = 1:length(node_index)
    if C(node_index(i)-1) == -1
        start_of_node_field = node_index(i) + 5; 
    end
end

for i = 1:length(triangle_index)
    if C(triangle_index(i)-1) == -1
        start_of_triangle_field = triangle_index(i) + 1; 
    end
end

for i = 1:length(lastfield_index)
    if C(lastfield_index(i)-1) == -1
        end_of_triangle_field = lastfield_index(i) - 2; 
    end
end

%if C(index(1)-1) == -1 %this is for small meshes, just in case less than 2412
 %   start_of_triangle_field = index(1) + 1; 
%end   

%nodes = zeros(numnodes,3)

i = start_of_node_field; % start i at 7
prealloc_length = ceil((start_of_triangle_field - 2 - start_of_node_field)/7); %algorithm to calc # nodes
nodes = zeros(prealloc_length,3); % preallocate matrix for speed
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
i = start_of_triangle_field + 6; % this is the first node in the triangle field
prealloc_length = ceil((end_of_triangle_field  - (start_of_triangle_field + 6))/9);
triangles = zeros(prealloc_length,3); % preallocate matrix for speed
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
%x = nodes(:,1); %just as before, no need for this if not using trimesh here
%y = nodes(:, 2); %this code is duplicated later, so its commented out here now
%z = nodes(:, 3);

%trimesh (triangles, x, y, z);


pts_A = zeros(length(triangles),3); %preallocate size for speed
pts_B = zeros(length(triangles),3); %preallocate size for speed
pts_C = zeros(length(triangles),3); %preallocate size for speed
vect1 = zeros(length(triangles),3); %preallocate size for speed
vect2 = zeros(length(triangles),3); %preallocate size for speed

for i = 1:length(triangles)
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

%x = normals(:,1); this code was only here for ploting with quiver
%y = normals(:, 2); if the normals dont need to be plotted
%z = normals(:, 3); then no need to break them up into x,y,z here

u = centroids(:,1);
v = centroids(:, 2);
q = centroids(:, 3);

%quiver3( u, v, q, x,y,z, 2);
axis equal;
xlabel('X');
ylabel('Y');
zlabel('Z');

%illuminate the sphere
torch = [0 0 -1]'; %completely arbitrary, this is the direction of the uniform vector field
torch = normr(torch);

illuminated = zeros(length(triangles),1); %preallocate size for speed
for i = 1:length(triangles)
    illuminated(i,:) = dot(torch, normals(i,:));
    if (illuminated(i, :) < -1e-14)
        illuminated(i,1) = 1;


    else
       illuminated(i, :) = 0;
    end
end

x = nodes(:,1);
y = nodes(:, 2);
z = nodes(:, 3);

%trisurf (triangles, x, y, z, illuminated); %use trisurf if you want to color faces

polarizing_vector = [1 0 0]'; %???? polarized

precalc_exponentials = zeros(length(normals),3); %preallocate for speed
%some lambdas found: sphere_veryhigh: .02. shape: .001
torch = torch'; % look into maybe not transposing in the first place. It must be like this to dot product with the centroids
for idx = 1:length(normals)
    precalc_exponentials(idx,:) = illuminated(idx,:) * polarizing_vector * exp(1i * (138*pi) * dot(torch(1,:), centroids(idx,:))); % 10pi for sphere_veryhigh, 200pi for shape.txt
end

%E_field_inc = real(precalc_exponentials);
E_field_inc = precalc_exponentials;

impedance = 120 * pi;
H_field_inc = zeros(length(E_field_inc),3);
for idx =1:length(E_field_inc)
    H_field_inc(idx,:) = cross(torch/impedance, E_field_inc(idx,:));
end

Jpo = zeros(length(H_field_inc),3);
for idx =1:length(H_field_inc)
    Jpo(idx,:) = 2*cross(normals(idx,:), H_field_inc(idx,:));
end

u = centroids(:,1);
v = centroids(:, 2);
q = centroids(:, 3);

%quiver3( u, v, q, x,y,z, 2);

magJpo = zeros(length(Jpo),1);
for idx =1:length(Jpo)
    magJpo(idx,:) = (sqrt(real(Jpo(idx,1))^2+real(Jpo(idx,2))^2 +real(Jpo(idx,3))^2));
end


x = nodes(:,1);
y = nodes(:, 2);
z = nodes(:, 3);

hold on

trisurf (triangles, x, y, z, magJpo, 'EdgeColor', 'none'); %use trisurf if you want to color faces
axis equal




% FOR NOW, K IS A RANDOM PLACEHOLDER FOR A CONSTANT. ANY USE OF K COULD BE
% REFERRING TO A DIFFERENT RANDOM CONSTANT
k = 5* pi; % I just chose a random constant. Figure it out later. k = omega * sqrt (mue * epsilon)
% O(N)
%calculate the g(r, r') at pt
scattering_field_x = zeros ((.025/.001), (.025/.001), (.025/.001));
scattering_field_y = zeros ((.025/.001), (.025/.001), (.025/.001));
scattering_field_z = zeros ((.025/.001), (.025/.001), (.025/.001));

greens_function = zeros(length(centroids),1);
test_plot_idx = 1;
for x_idx = 1:1:7
for y_idx = 1:1:7
for z_idx = 1:1:7

pre_scattering_field = [0, 0, 0];
pt = [(x_idx-1)/200, (y_idx-1)/200, (z_idx-1)/200];
for idx = 1:length(centroids)
    r = cell2mat({centroids(idx,1) - pt(1, 1), centroids(idx,2) - pt(1, 2), centroids(idx,3) - pt(1, 3)});
    distance = sqrt(real(r(1, 1))^2 + real(r(1, 2))^2 + real(r(1, 3))^2);
    g = (exp(1i * 2*pi * k * distance) / (4 * pi * distance)); 
    greens_function = eye(3) * g - (1/k^2) * del2 (g, .0001) ;
    pre_scattering_field = pre_scattering_field + dot (greens_function, Jpo (idx, :));
end

scattering_field_x (x_idx, y_idx, z_idx)  = pre_scattering_field(1,1) * 1i * k; % it shouldn't be k. Replace k with 1i * omega * mue 
scattering_field_y (x_idx, y_idx, z_idx)  = pre_scattering_field(1,2) * 1i * k; % it shouldn't be k. Replace k with 1i * omega * mue 
scattering_field_z (x_idx, y_idx, z_idx)  = pre_scattering_field(1,3) * 1i * k; % it shouldn't be k. Replace k with 1i * omega * mue 

scattering_field_for_plot (1, test_plot_idx) = scattering_field_x (x_idx, y_idx, z_idx);
scattering_field_for_plot (2, test_plot_idx) = scattering_field_x (x_idx, y_idx, z_idx);
scattering_field_for_plot (3, test_plot_idx) = scattering_field_x (x_idx, y_idx, z_idx);

x_for_plot(test_plot_idx) = (x_idx - 1) / 200;
y_for_plot(test_plot_idx) = (y_idx - 1) / 200;
z_for_plot(test_plot_idx) = (z_idx - 1) / 200;
test_plot_idx = test_plot_idx + 1;


end
end
end

hold on
quiver3( x_for_plot, y_for_plot, z_for_plot, real(scattering_field_for_plot (:, 1)), real(scattering_field_for_plot (:, 2)), real(scattering_field_for_plot (:, 3)), 3);

trisurf (triangles, x, y, z, magJpo, 'EdgeColor', 'none'); %use trisurf if you want to color faces



