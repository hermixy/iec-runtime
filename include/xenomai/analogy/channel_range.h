/**
 * @file
 * Analogy for Linux, channel, range related features
 *
 * Copyright (C) 1997-2000 David A. Schleef <ds@schleef.org>
 * Copyright (C) 2008 Alexis Berlemont <alexis.berlemont@free.fr>
 *
 * Xenomai is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Xenomai is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Xenomai; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __ANALOGY_CHANNEL_RANGE__
#define __ANALOGY_CHANNEL_RANGE__

#if __GNUC__ >= 3
#define GCC_ZERO_LENGTH_ARRAY
#else
#define GCC_ZERO_LENGTH_ARRAY 0
#endif

/*!
 * @ingroup driverfacilities
 * @defgroup channelrange Channels and ranges
 *
 * Channels
 *
 * According to the Analogy nomenclature, the channel is the elementary
 * acquisition entity. One channel is supposed to acquire one data at
 * a time. A channel can be:
 * - an analog input or an analog ouput;
 * - a digital input or a digital ouput;
 *
 * Channels are defined by their type and by some other
 * characteristics like:
 * - their resolutions for analog channels (which usually ranges from
     8 to 32 bits);
 * - their references;
 *
 * Such parameters must be declared for each channel composing a
 * subdevice. The structure a4l_channel (a4l_chan_t) is used to
 * define one channel.
 *
 * Another structure named a4l_channels_desc (a4l_chdesc_t)
 * gathers all channels for a specific subdevice. This latter
 * structure also stores :
 * - the channels count;
 * - the channels declaration mode (A4L_CHAN_GLOBAL_CHANDESC or
     A4L_CHAN_PERCHAN_CHANDESC): if all the channels composing a
     subdevice are identical, there is no need to declare the
     parameters for each channel; the global declaration mode eases
     the structure composition.
 *
 * Usually the channels descriptor looks like this:
 * <tt> @verbatim
a4l_chdesc_t example_chan = {
	mode: A4L_CHAN_GLOBAL_CHANDESC, -> Global declaration
					      mode is set
	length: 8, -> 8 channels
	chans: {
		{A4L_CHAN_AREF_GROUND, 16}, -> Each channel is 16 bits
						  wide with the ground as
						  reference
	},
};
@endverbatim </tt>
 *
 * Ranges
 *
 * So as to perform conversion from logical values acquired by the
 * device to physical units, some range structure(s) must be declared
 * on the driver side.
 *
 * Such structures contain:
 * - the physical unit type (Volt, Ampere, none);
 * - the minimal and maximal values;
 *
 * These range structures must be associated with the channels at
 * subdevice registration time as a channel can work with many
 * ranges. At configuration time (thanks to an Analogy command), one
 * range will be selected for each enabled channel.
 *
 * Consequently, for each channel, the developer must declare all the
 * possible ranges in a structure called a4l_rngtab_t. Here is an
 * example:
 * <tt> @verbatim
a4l_rngtab_t example_tab = {
    length: 2,
    rngs: {
	RANGE_V(-5,5),
	RANGE_V(-10,10),
    },
};
@endverbatim </tt>
 *
 * For each subdevice, a specific structure is designed to gather all
 * the ranges tabs of all the channels. In this structure, called
 * a4l_rngdesc_t, three fields must be filled:
 * - the declaration mode (A4L_RNG_GLOBAL_RNGDESC or
 *   A4L_RNG_PERCHAN_RNGDESC);
 * - the number of ranges tab;
 * - the tab of ranges tabs pointers;
 *
 * Most of the time, the channels which belong to the same subdevice
 * use the same set of ranges. So, there is no need to declare the
 * same ranges for each channel. A macro is defined to prevent
 * redundant declarations: RNG_GLOBAL().
 *
 * Here is an example:
 * <tt> @verbatim
a4l_rngdesc_t example_rng = RNG_GLOBAL(example_tab);
@endverbatim </tt>
 *
 * @{
 */


/* --- Channel section --- */

/*!
 * @anchor A4L_CHAN_AREF_xxx @name Channel reference
 * @brief Flags to define the channel's reference
 * @{
 */

/**
 * Ground reference
 */
#define A4L_CHAN_AREF_GROUND 0x1
/**
 * Common reference
 */
#define A4L_CHAN_AREF_COMMON 0x2
/**
 * Differential reference
 */
#define A4L_CHAN_AREF_DIFF 0x4
/**
 * Misc reference
 */
#define A4L_CHAN_AREF_OTHER 0x8

	  /*! @} A4L_CHAN_AREF_xxx */

/**
 * Internal use flag (must not be used by driver developer)
 */
#define A4L_CHAN_GLOBAL 0x10

/*!
 * @brief Structure describing some channel's characteristics
 */

struct a4l_channel {
	unsigned long flags; /*!< Channel flags to define the reference. */
	unsigned long nb_bits; /*!< Channel resolution. */
};
typedef struct a4l_channel a4l_chan_t;

/*!
 * @anchor A4L_CHAN_xxx @name Channels declaration mode
 * @brief Constant to define whether the channels in a descriptor are
 * identical
 * @{
 */

/**
 * Global declaration, the set contains channels with similar
 * characteristics
 */
#define A4L_CHAN_GLOBAL_CHANDESC 0
/**
 * Per channel declaration, the decriptor gathers differents channels
 */
#define A4L_CHAN_PERCHAN_CHANDESC 1

	  /*! @} A4L_CHAN_xxx */

/*!
 * @brief Structure describing a channels set
 */

struct a4l_channels_desc {
	unsigned long mode; /*!< Declaration mode (global or per channel) */
	unsigned long length; /*!< Channels count */
	a4l_chan_t chans[GCC_ZERO_LENGTH_ARRAY]; /*!< Channels tab */
};
typedef struct a4l_channels_desc a4l_chdesc_t;

/* --- Range section --- */

/** Constant for internal use only (must not be used by driver
    developer).  */
#define A4L_RNG_FACTOR 1000000

/**
 * Volt unit range flag
 */
#define A4L_RNG_VOLT_UNIT 0x0
/**
 * MilliAmpere unit range flag
 */
#define A4L_RNG_MAMP_UNIT 0x1
/**
 * No unit range flag
 */
#define A4L_RNG_NO_UNIT 0x2
/**
 * External unit range flag
 */
#define A4L_RNG_EXT_UNIT 0x4
/**
 * Macro to retrieve the range unit from the range flags
 */
#define A4L_RNG_UNIT(x) (x & (A4L_RNG_VOLT_UNIT |	\
			      A4L_RNG_MAMP_UNIT |	\
			      A4L_RNG_NO_UNIT |		\
			      A4L_RNG_EXT_UNIT))

/**
 * Internal use flag (must not be used by driver developer)
 */
#define A4L_RNG_GLOBAL 0x8

/*!
 * @brief Structure describing a (unique) range
 */

struct a4l_range {
	long min; /*!< Minimal value */
	long max; /*!< Maximal falue */
	unsigned long flags; /*!< Range flags (unit, etc.) */
};
typedef struct a4l_range a4l_rng_t;

/**
 * Macro to declare a (unique) range with no unit defined
 */
#define RANGE(x,y) {(x * A4L_RNG_FACTOR), (y * A4L_RNG_FACTOR),	\
			A4L_RNG_NO_UNIT}
/**
 * Macro to declare a (unique) range in Volt
 */
#define RANGE_V(x,y) {(x * A4L_RNG_FACTOR),(y * A4L_RNG_FACTOR), \
			A4L_RNG_VOLT_UNIT}
/**
 * Macro to declare a (unique) range in milliAmpere
 */
#define RANGE_mA(x,y) {(x * A4L_RNG_FACTOR),(y * A4L_RNG_FACTOR), \
			A4L_RNG_MAMP_UNIT}
/**
 * Macro to declare a (unique) range in some external reference
 */
#define RANGE_ext(x,y) {(x * A4L_RNG_FACTOR),(y * A4L_RNG_FACTOR), \
			A4L_RNG_EXT_UNIT}


/* Ranges tab descriptor */
#define A4L_RNGTAB(x)				\
	struct {				\
		unsigned char length;		\
		a4l_rng_t rngs[x];		\
	}
typedef A4L_RNGTAB(GCC_ZERO_LENGTH_ARRAY) a4l_rngtab_t;

/**
 * Constant to define a ranges descriptor as global (inter-channel)
 */
#define A4L_RNG_GLOBAL_RNGDESC 0
/**
 * Constant to define a ranges descriptor as specific for a channel
 */
#define A4L_RNG_PERCHAN_RNGDESC 1

/* Global ranges descriptor */
#define A4L_RNGDESC(x)				\
	struct {				\
		unsigned char mode;		\
		unsigned char length;		\
		a4l_rngtab_t *rngtabs[x];	\
	}
typedef A4L_RNGDESC(GCC_ZERO_LENGTH_ARRAY) a4l_rngdesc_t;

/**
 * Macro to declare a ranges global descriptor in one line
 */
#define RNG_GLOBAL(x) {				\
	.mode = A4L_RNG_GLOBAL_RNGDESC, 	\
	.length =  1,				\
	.rngtabs = {&(x)}, }

extern a4l_rngdesc_t a4l_range_bipolar10;
extern a4l_rngdesc_t a4l_range_bipolar5;
extern a4l_rngdesc_t a4l_range_unipolar10;
extern a4l_rngdesc_t a4l_range_unipolar5;
extern a4l_rngdesc_t a4l_range_unknown;
extern a4l_rngdesc_t a4l_range_fake;

#define range_digital a4l_range_unipolar5

	  /*! @} channelrange */

#endif /* __ANALOGY_CHANNEL_RANGE__ */
