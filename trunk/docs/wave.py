from numpy import *
from time import clock

#import matplotlib
#matplotlib.use('WXAgg') # do this before importing pylab
import pylab as pl
import cv

#from enthought.mayavi import mlab


g = 9.8
pho = 1000.0
Uw = 15.0

n = 512

max_frequency_rad = 6.0
max_wavenumber_rad = max_frequency_rad**2 / g

D = 2*pi / ( max_wavenumber_rad / (n/2) )
print 'spatial period:', D

def spectPM(w):
    return 0.0081*g*g * w**(-5) * exp(-0.74 * (w*Uw/g)**(-4) )
def spectPM_k(k):
    w = sqrt( k*g )
    return 0.5 * spectPM(w) * g / w

def amplitude1d(k):
    dk = max_wavenumber_rad / (n/2)
    return sqrt( 2*spectPM_k(k)*dk )

def amplitude2d(kx, ky):
    k = sqrt(kx*kx + ky*ky)
    c = maximum(0.0, kx / k)

    narrow_coef = 16.0
    e = amplitude1d(k) * c**narrow_coef / k
    
    dk = max_wavenumber_rad / (n/2)
    return sqrt(2*e*dk*dk)

phase1d = exp(2j*pi*random.rand(n))
phase2d = exp(2j*pi*random.rand(n, n))

def spec2d(t):
    k = arange(0, 2*max_wavenumber_rad, max_wavenumber_rad / (n/2))
    k[n/2+1:] -= 2*max_wavenumber_rad
    kx, ky = meshgrid(k, k)

    amp = amplitude2d(kx, ky)
    amp[~isfinite(amp)] = 0.0

    k = sqrt(kx*kx + ky*ky)
    w = sqrt( k*g )

    return amp * phase2d * exp(-2j*pi*w*t) * (n*n)

def wave(t):
    k = arange(0, 2*max_wavenumber_rad, max_wavenumber_rad / (n/2))
    k[n/2+1:] -= 2*max_wavenumber_rad

    amp = amplitude1d(k)
    amp[~isfinite(amp)] = 0.0
    amp[n/2+1:] = 0

    w = sqrt( abs(k)*g )
    spec = amp * phase1d * exp(-2j*pi*w*t) * n
    
    return real( fft.ifft(spec) )


def wave2d(t):
    spec = spec2d(t)
    start = clock()
    h = fft.ifft2(spec)
    print (clock() - start) * 1000, 'ms'

    return real( h )


def test_anim():
    x = linspace(0, D, n)

    fig = pl.figure()
    ax = fig.add_subplot(111)
    line = ax.plot(x, wave(0.0))[0]
    #ax.axis('equal')

    text = ax.annotate('t = %f' % 0.0, (0.1, 0.9), xycoords='axes fraction')

    def update_line(idleevent):
        #if update_line.i==200:
        #    return False
        #print 'animate', update_line.i
        line.set_ydata( wave(update_line.t) )
        text.set_text('t = %f' % update_line.t)
        fig.canvas.draw_idle()                 # redraw the canvas
        update_line.t += 0.05
    update_line.t = 0

    import wx
    wx.EVT_IDLE(wx.GetApp(), update_line)

    pl.show()

def test_2d_anim():
    for i in xrange(100):
        h = wave2d(0.2 * i)
        h = uint8( clip(h / 2.0 + 0.5, 0, 1)*255 )
        cv.SaveImage('out/h_%03d.bmp' % i, h)
        print i

def test_2d():
    h = wave2d(0)
    pl.imshow(h)
    pl.colorbar()
    pl.show()
        
def verify2d():
    time_step = 0.25
    total_time = time_step*2048
    for i in xrange(int(total_time / time_step)):
        print '---------', i
        h = real( wave2d(i*time_step) )[::16, ::16]
        save('out/h16_%04d.npy' % i, h)
        
def verify1d():
    time_step = 0.25
    step_n = 2048
    h = zeros((step_n, n))
    for i in xrange(step_n):
        t = i*time_step
        h[i] = wave(t)

    
    #total_time = time_step
    A = abs( fft.rfft(h, axis=0) ).mean(1)

    
    pl.plot(A)
    pl.show()
    

if __name__ == '__main__':
   # verify1d()
   test_2d_anim()
