double
nanmean(const double *a, const unsigned int n) {
    unsigned int i, k;
    double s;
    s = 0;
    k = 0;
    for(i = 0; i < n; i++) {
        if(isnan(a[i]))
            continue;
        s += a[i];
        k++;
    }
    s /= k;
    return s;
}

void
cdfuse(double *t,
    double *s,
    const unsigned int u,
    const unsigned int v,
    const unsigned int w,
    const unsigned int *x,
    const unsigned int n,
    const unsigned int niter,
    const double etol) {
    unsigned int i, j, k, l, dc, nn;
    unsigned int ijk, filled;
    double *a, dt;

    filled = 0;
    for(l = 0; l < niter; l++) {
#pragma omp parallel for private(i,j,k,a)
        for(i = 1; i < u-1; i++) {
            a = malloc(7 * sizeof(double));
            if(!a)
                exit(EXIT_FAILURE);
            for(j = 1; j < v-1; j++) {
                for(k = 1; k < w-1; k++) {
                    a[0] = s[i*v*w+j*w+k];
                    a[1] = s[(i-1)*v*w+j*w+k];
                    a[2] = s[(i+1)*v*w+j*w+k];
                    a[3] = s[i*v*w+(j-1)*w+k];
                    a[4] = s[i*v*w+(j+1)*w+k];
                    a[5] = s[i*v*w+j*w+k-1];
                    a[6] = s[i*v*w+j*w+k+1];
                    t[i*v*w+j*w+k] = nanmean(a, 7);
                }
            }
            free(a);
        }
#pragma omp parallel for private(i)
        for(i = 0; i < n; i++)
            t[x[i]] = s[x[i]];
        if(niter >= 10000) {
            nn = 1;
            if(filled) {
                dt = dc = 0;
                for(i = 1; i < u-1; i++) {
                    for(j = 1; j < v-1; j++) {
                        for(k = 1; k < w-1; k++) {
                            ijk = i*v*w+j*w+k;
                            dt += fabs(s[ijk] - t[ijk]);
                            dc++;
                        }
                    }
                }
            } else {
                for(i = 1; i < u-1; i++) {
                    for(j = 1; j < v-1; j++) {
                        for(k = 1; k < w-1; k++) {
                            ijk = i*v*w+j*w+k;
                            if(isnan(s[ijk])) {
                                nn = 0;
                                break;
                            }
                        }
                    }
                }
            }
            if(filled)
                if(dt/dc < etol)
                    break;
            if(nn)
                filled = 1;
        }
        memcpy(s, t, u * v * w * sizeof(double));
    }
}
