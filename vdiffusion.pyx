import numpy as np

__version__ = 0.1
__author__ = 'Aljoscha Rheinwalt <aljoscha.rheinwalt@uni-potsdam.de>'

cdef extern from "cdiffuse.c":
    void cdfuse(double *t, double *s,
                const unsigned int u,
                const unsigned int v,
                const unsigned int w,
                const unsigned int *x,
                const unsigned int n,
                const unsigned int niter,
                const double etol);

def diffuse(vol, iterations = -1, tolerance = 1e-5):
    """
    out = diffuse(vol)

    Parameters
    ----------
    vol : ndarray
        A 3D Numpy array with NaNs for missing, signed scalar field values.

    Returns
    -------
    out : ndarray
    """
    cdef unsigned int niter
    cdef double etol = tolerance
    if iterations <= 0:
        niter = 10000
    else:
        niter = iterations
    if not vol.flags['C_CONTIGUOUS']:
        vol = np.ascontiguousarray(vol)
    vol = np.pad(vol.astype(np.float64), 1, constant_values = np.nan)
    ret = np.nan * np.ones(vol.shape)
    cdef const unsigned int[::1] vi = np.where(~np.isnan(vol).ravel())[0].astype(np.uint32)
    cdef double[:,:,::1] vs = vol
    cdef double[:,:,::1] vt = ret
    cdfuse(&vt[0,0,0], &vs[0,0,0], vs.shape[0], vs.shape[1], vs.shape[2], &vi[0], vi.shape[0], niter, etol)
    return ret[1:-1,1:-1,1:-1]

