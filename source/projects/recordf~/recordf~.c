// updated for Max 7 by Darwin Grosse and Tim Place
// ------------------------------------------------

#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"
#include "buffer.h"

t_class *recordf_class;

typedef struct _recordf
{
    t_pxobject l_obj;
    t_symbol *l_sym;
    t_buffer *l_buf;
    long l_chan;
    long length;
    long loop_start, loop_end;
    long index;
    long srate;
    float srate_ms;
    short record;
    short loop;
    short reset;
    short append;

} t_recordf;

long Constrain(long v, long lo, long hi);
void recordf_set(t_recordf *x, t_symbol *s);
void *recordf_new(t_symbol *s, long chan);
void recordf_in1(t_recordf *x, long n);
void recordf_in2(t_recordf *x, long n);
void recordf_in0(t_recordf *x, long n);
void record_loop(t_recordf *x, t_symbol *s, long argc, t_atom *argv);
void record_goto(t_recordf *x, t_symbol *s, long argc, t_atom *argv);
void record_reset(t_recordf *x, t_symbol *s, long argc, t_atom *argv);
void record_append(t_recordf *x, t_symbol *s, long argc, t_atom *argv);
void recordf_assist(t_recordf *x, void *b, long m, long a, char *s);
void recordf_dblclick(t_recordf *x);

// dsp stuff
void recordf_dsp64(t_recordf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void recordf_perform64(t_recordf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_symbol *ps_buffer;

void ext_main(void* p)
{
    t_class *c = class_new("recordf~", (method)recordf_new, (method)dsp_free, (long)sizeof(t_recordf), 0L, A_SYM, A_DEFLONG, 0);
    
    class_addmethod(c, (method)recordf_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)recordf_dsp64, "dsp64", A_CANT, 0);
    
    class_addmethod(c, (method)recordf_in1, "in1", A_LONG, 0);
    class_addmethod(c, (method)recordf_in2, "in2", A_LONG, 0);
    class_addmethod(c, (method)recordf_in0, "int", A_LONG, 0);
    
    class_addmethod(c, (method)recordf_set, "set", A_SYM, 0);
    class_addmethod(c, (method)recordf_dblclick, "dblclick", A_CANT, 0);
	class_addmethod(c, (method)record_reset, "reset", A_GIMME, 0);
	class_addmethod(c, (method)record_loop, "loop", A_GIMME, 0);
	class_addmethod(c, (method)record_goto, "goto", A_GIMME, 0);
	class_addmethod(c, (method)record_append, "append", A_GIMME, 0);
    class_dspinit(c);
    
    class_register(CLASS_BOX, c);
    recordf_class = c;
	ps_buffer = gensym("buffer~");
}

void *recordf_new(t_symbol *s, long chan)
{
	t_recordf *x = (t_recordf *)object_alloc(recordf_class);
	//dsp_setup((t_pxobject *)x, 1);
    intin((t_object *)x, 3);
    intin((t_object *)x, 2);
	dsp_setup((t_pxobject *)x, 2);
	outlet_new((t_object *)x, "signal");
	x->l_sym = s;
	x->index = 0;
	x->record = 0;
	x->reset = 0;
	x->loop_start = 0;
	x->loop_end = 100;
	x->l_chan = 0;
	x->loop = 0;
	
	recordf_set(x, s);
	//recordf_in1(x,chan);
	//x->l_obj.z_misc = Z_NO_INPLACE;
	return (x);
}

void recordf_assist(t_recordf *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET) {
		switch (a) {
            case 0:
                sprintf(s,"record on/off");
                break;
            case 1:
                sprintf(s,"loop start");
                break;
            case 2:
                sprintf(s,"loop end");
                break;
        }
	} else {
		sprintf(s,"(signal) output");
    }
}

void recordf_dblclick(t_recordf *x)
{
	t_buffer *b;
	
	if ((b = (t_buffer *)(x->l_sym->s_thing)) && ob_sym(b) == ps_buffer)
		mess0((struct object *)b,gensym("dblclick"));
}

long Constrain(long v, long lo, long hi)
{
	if (v < lo)
		return lo;
	else if (v > hi)
		return hi;
	else
		return v;
}

void recordf_set(t_recordf *x, t_symbol *s)
{
	t_buffer *b;
	
	x->l_sym = s;
	if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer) {
		x->l_buf = b;
		x->loop_end = b->b_frames;
		x->loop_start  = 0;
		//post("recordf~: setting record end point to %d", x->loop_end);
	} else {
		error("recordf~: no buffer~ %s", s->s_name);
		x->l_buf = 0;
	}
}

void recordf_in1(t_recordf *x, long n)
{
	x->loop_start = n;
	//post("recordf: loopstart = %d ", n);
}

void recordf_in2(t_recordf *x, long n)
{
	x->loop_end = n;
	//post("recordf: loopend = %d ", n);
}


void recordf_in0(t_recordf *x, long n)
{
	x->record = n;
	if(!x->append) x->index = 0; //reset recording point to beginning of loop
	//post("recordf: recording = %d ", n);
}

void record_loop(t_recordf *x, t_symbol *s, long argc, t_atom *argv)
{
	short i;
	int temp;
	for (i=0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				temp = (int)argv[i].a_w.w_long;
				x->loop = temp;
				break;
			case A_FLOAT:
				temp = (int)argv[i].a_w.w_long;
				x->loop = temp;
				break;
		}
	}
}

void record_goto(t_recordf *x, t_symbol *s, long argc, t_atom *argv)
{
	float temp;
    switch (argv[1].a_type) {
        case A_LONG:
            temp = (float)argv[1].a_w.w_long;
            break;
        case A_FLOAT:
            temp = argv[1].a_w.w_float;
            break;
            
	}
	temp *= x->srate_ms;
	x->index = (long)(temp);
	//post("recordf~: setting new record point to %ld", x->index);
}

void record_append(t_recordf *x, t_symbol *s, long argc, t_atom *argv)
{
	short i;
	int temp;
	for (i=0; i < argc; i++) {
		switch (argv[i].a_type) {
			case A_LONG:
				temp = (int)argv[i].a_w.w_long;
				x->append = (long)((float)temp * x->srate_ms);
				break;
			case A_FLOAT:
				temp = (int)argv[i].a_w.w_long;
				x->append = (long)((float)temp * x->srate_ms);
				break;
		}
	}
}

void record_reset(t_recordf *x, t_symbol *s, long argc, t_atom *argv)
{
	x->reset = 1;
}

void recordf_dsp64(t_recordf *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
    x->srate = samplerate;
    x->srate_ms = (float)x->srate * .001;
    recordf_set(x,x->l_sym);
    
    object_method(dsp64, gensym("dsp_add64"), x, recordf_perform64, 0, NULL);
}

void recordf_perform64(t_recordf *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
    t_double *in = (t_double *)(ins[0]);
    t_double *fcoeff = (t_double *)(ins[1]);
    t_double *out = (t_double *)(outs[0]);
    int n = sampleframes;
    
	t_buffer *b = x->l_buf;
	float *tab;
    
	t_double input, coeff;
	t_double chan, frames, nc, one_over_length;
	long index;
	t_double loop_start, loop_end;
	
	if (x->l_obj.z_disabled)
		return;
	if (!b)
		return;
	if (!b->b_valid)
		return;
	
	tab = b->b_samples;
	chan = (t_double)x->l_chan;
	frames = (t_double)b->b_frames;
	nc = (t_double)b->b_nchans;
	
	if(!x->reset) {
		loop_start = (t_double)x->loop_start * x->srate_ms;
		loop_end = (t_double)x->loop_end * x->srate_ms;
		
		if (loop_end < 0) loop_end = frames;
		else if (loop_end > frames) loop_end = frames;
		if (loop_start < 0) loop_start = 0;
		else if (loop_start >= loop_end) loop_start = 0;
		if (loop_end <= loop_start) loop_end = frames;
	} else {
		loop_start = 0;
		loop_end = frames;
		x->reset = 0;
	}
	
	x->length = loop_end - loop_start;
	if (x->length != 0) one_over_length = 1./(t_double)x->length;
	else one_over_length = 1./frames;
	
	if(x->record) {
		while (n--) {
			input 	= *in++;
			coeff 	= *fcoeff++;
            
			*out++ = ((t_double)x->index - loop_start) * one_over_length; //needs to be sync val (0-1)
			
			if(x->loop) {
				if (x->index >= loop_end) x->index = loop_start;
				if (x->index < loop_start) x->index = loop_start;
				index = x->index*nc + chan;
				
				tab[index] = coeff * tab[index] + input;
				x->index++;
			} else {
				if (x->index < loop_end) {
					index = x->index*nc + chan;
					tab[index] = coeff * tab[index] + input;
					x->index++;
				} else x->record = 0;
			}
            
		}
	} else while (n--) *out++ =  ((t_double)x->index - loop_start) * one_over_length; //needs to be sync val (0-1);
}