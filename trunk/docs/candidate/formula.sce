
clf();

x= [0,1]
y = [0,1];
a = gca();
a.data_bounds = [0,0;10,10]

plot2d(x,y);

xstring(3,7,"$\LARGE\mathbf{F} =-\left[ \iint\limits_{S} p \mathbf{n} dS 	\right]-\left[ \iint\limits_{S} H \mathbf{v} dS 	\right]+ \mathbf{D}+ \mathbf{F}_{ext}$")

xstring(3,5,"$\LARGE\mathbf{M} =-\left[ \iint\limits_{S} \left( p \mathbf{n} \right) \times \left( \mathbf{r} - \mathbf{p} \right) dS	\right]-\left[ \iint\limits_{S} \left( H \mathbf{v} \right) \times \left( \mathbf{r} - \mathbf{p} \right) dS	\right]	+ \mathbf{M}_{ext}$")

xstring(3,3,"$\LARGE h_w(t,\mathbf{p},d) = \sum_{i,j}\exp \left( -|\mathbf{k}_{i,j}| d \right)a_{i,j}\cos(\mathbf{p}\cdot \mathbf{k}_{i,j} - \omega(\mathbf{k}_{i,j})t + \delta_{i,j})$");

xstring(3,1,"$\LARGE p(t,\mathbf{p}, d) = \gamma h_w(t, \mathbf{p}, d) + d \gamma$");

xs2pdf(gcf(), "E:/formulas.pdf")		