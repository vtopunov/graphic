#pragma once

#include <map>
#include <QWidget>
#include <QtGui>
#include "GraphicLimits.h"
#include "GraphicLine.h"
#include "num_sequence.h"

#undef min
#undef max

typedef ::num_sequence<long double> grid_t;

class GraphicWidget;

inline bool scale(limits& lim, limits::real_t speed_scale, limits::real_t nScale = 1) {
    const auto x0 = lim.mean();
    const auto dx = ( lim.length() * powl(1 + speed_scale, nScale ) ) / 2;
    return lim.set(x0-dx, x0+dx);
}

struct x_axis_type_tag {};
struct y_axis_type_tag {};

inline const double* garray(const GraphicLine& ln, x_axis_type_tag) { return ln.x; }
inline const double* garray(const GraphicLine& ln, y_axis_type_tag) { return ln.y; }

inline limits unilimits(const limits& lim, x_axis_type_tag ) { return lim; }
inline limits unilimits(const limits& lim, y_axis_type_tag ) { return lim.inv(); }

inline grid_t grid(const limits& lim, const limits::real_t h) {
    return grid_t( ceill(lim.min()/h) * h, h, lim.max() );
}

inline limits::real_t h_round_grid(const limits& lim, limits::real_t round) {
    return powl(10., static_cast<int>( log10l( round * lim.length() ) ) );
}

inline grid_t round_grid(const limits& lim, limits::real_t round ) {
    const auto h = powl(10., static_cast<int>( log10l( round * lim.length() ) ) );
	return grid(lim, h);
}

inline grid_t grid(const limits& lim, double minngrid, double maxngrid) { // Определение параметров сетки 
    auto h = h_round_grid(lim, 0.1); // Шаг сетки
	if( lim.length() / h < minngrid ) h*=0.1; // Для того чтобы небыло, слишком мало линий
	if( lim.length() / h > maxngrid ) h*=10.; // Для того чтобы небыло, cлишком много линий
    return grid_t( ceill(lim.min()/h) * h, h, lim.max() );
}


template<class It>
inline QSizeF maxGraphicSizeText(It begin, It end, const QFontMetricsF& tm ) {
	double w = 0;
	double maxw = tm.width("000");
	for(; begin != end; ++begin ) {
		w = tm.width( begin->second );
		if( w > maxw ) maxw = w;
	}
	return QSize(maxw, tm.height());
}

typedef std::map<double, QString> labelsmap_t;
inline bool addlabel(labelsmap_t& map, double value, QString text, double eps = 1.0e-5) {
	if( map.empty() ) { map[value] = text; return true; }

    auto lb = map.lower_bound( value );
    auto it = lb;
	if( it != map.end() ) {
        if( std::fabs(it->first - value) < eps ) return false; // элемент уже есть
	}
	if( it != map.begin() ) {
        if( std::fabs((--it)->first - value) < eps ) return false; // элемент уже есть
	}

	map.insert(lb, labelsmap_t::value_type(value, text) );
	return true;
}

class GraphicWidget;

template<class axis_type>
struct GraphicAxis {
public:
	enum{ labeldiv = 100 };

public:
	QPen gridpen_;
	std::map<QString, QPen> gridpens_;
	QPen textpen_;
	QFont textfont_;
    double textangle_;
	bool autogrid_;
	bool showlable_;
	double maxngrid_;
	double minngrid_;
	QString labelpostfix_;
	QString labelprefix_;

private:
	labelsmap_t labels_;
	labelsmap_t text_;
	std::map<QString, QPen> gridpens__;

public:
    limits::real_t speed_scale_;
    limits::real_t speed_move_;

private:
    limits real_;
	limits window_;
	limits save_real_;

public:
    GraphicAxis() : gridpen_(), gridpens_(), textpen_(), textfont_(),
		textangle_(), autogrid_(), maxngrid_(), minngrid_(), labelpostfix_(),
        labelprefix_(), labels_(), text_(), gridpens__(),
        speed_scale_(), speed_move_(), real_(), window_(), save_real_() { clear(); }

	void clear() {
		gridpen_ = QColor(0,255,0);
		textpen_ = QColor(0,0,0);
		textfont_ = QFont("MS Sans Serif", 8, 50, true),
        textangle_ = 0;
		autogrid_ = true;
		showlable_ = true;
		labelpostfix_ = "";
		labelprefix_ = "";

        labels_ = decltype(labels_)();
		text_ = decltype(text_)();
		gridpens_ = decltype(gridpens_)();
		speed_scale_ = 0.1; 
		speed_move_ = 1.0;
		clear_line();
		maxngrid_ = 20;
		minngrid_ = 5;
	}
	
    void real(limits::real_t min, limits::real_t max ) { real(limits(min, max) ); }

	void real(const limits& lim) {
		if( lim.valid() ) {
			real_ = lim;
			save_limits();
		}
	}

	const limits& real()const { return real_; }

    void window(const limits& window__) { window_ = unilimits(window__, axis_type()); }
    limits window() { return unilimits(window_, axis_type()); }

    limits::real_t to_real(limits::real_t x) { return window_.to(real_, x); }
    limits::real_t to_window(limits::real_t x) { return real_.to(window_, x); }

	void clear_line() {
		real_ = limits();
		window_ = limits();
		save_real_ = limits();
	}

    static constexpr limits::real_t addlimmin(x_axis_type_tag) { return 0.01; }

    static constexpr limits::real_t addlimmin(y_axis_type_tag) { return 0.02; }

    template<class type>
	static constexpr limits::real_t addlimmax(type) { return 0.01; }

    void addline(const GraphicLine& ln, const limits& exlim = limits() ) {
        if( valid(ln) ) {
            auto g = garray(ln, axis_type());
            auto minmax = std::minmax_element(g, g+ln.n);
            limits glim(*minmax.first, *minmax.second);
            bool is_set = false;

            if( glim.min() < real_.min() ) { is_set = true; } else { glim.min_ = real_.min(); }
            if( glim.max() > real_.max() ) { is_set = true; } else { glim.max_ = real_.max(); }

            if( is_set ) {
                const auto addabsmax = addlimmax(axis_type()) * glim.length();
                const auto addabsmin = addlimmin(axis_type()) * glim.length();
                glim.max_ += addabsmax;
                glim.min_ -= addabsmin;

                if( exlim.valid() ) {
                    glim.max_ = std::max(glim.max_, exlim.max() + addabsmax );
                    glim.min_ = std::min(glim.min_, exlim.min() - addabsmin );
                }

                real(glim);
            }
        }
	}


    void move(limits::real_t dx) {
		real_.set(to_real(window_.min() + speed_move_ * dx ), to_real(window_.max() + speed_move_ * dx ) );
	}

    bool scale(limits::real_t nScale = 1) { return ::scale(real_, speed_scale_, nScale); }

	void save_limits() { save_real_ = real_; }
	void restore_limits() { real_ = save_real_; }

	bool addlabel(QString value, QString text) {
        if( ( value.isEmpty() && text.isEmpty() ) || !real_.valid() ) return false;
		if( text.isEmpty() )  { text = value; }
		if( value.isEmpty() ) { value = text; };
        bool ok = false;
        double real_value = value.toDouble(&ok);
        return ok && ::addlabel(labels_, real_value, text, real_.length() / labeldiv );
	}

	const labelsmap_t& labels()const { return labels_; }
	const QFont& font()const { return textfont_; }

	void prepaint() { textinit(); }

	QPen gridpen(const QString& lb) {
		auto it = gridpens__.find(lb);
		return ( it == gridpens__.end() ) ? gridpen_ : it->second;	
	}

private:
	void textinit() {
		text_ = labels_;
		gridpens__ = gridpens_;
		if(autogrid_) {
			auto grid = ::grid(real_, minngrid_, maxngrid_);
			auto mega_grid = ::grid(real_, grid.increment() * 10 );
			QPen megapen( gridpen_ );
			megapen.setWidth( megapen.width() + 1 );
            for(auto x : mega_grid ) {
                gridpens__.insert( std::make_pair(to_string(x), megapen) );
			}

            for(auto x : grid ) {
                ::addlabel(text_, x, to_string(x), real_.length() / labeldiv );
			}
		}

		for(auto it = text_.begin(); it != text_.end(); ++it ) {
			it->second = labelprefix_ + it->second + labelpostfix_;
		}

	}

	const labelsmap_t& text()const { return text_; }

    static QString to_string(long double value) {
        auto format = ( fabs(value) < 1.0e7 ) ? 'f' : 'g';
        auto s = QString::number(value, format);
        if( format == 'f' && !s.isEmpty() ) {
            auto pos = s.end()-1;
            int nchop = 0;
            for(; (pos != s.begin()) && (*pos == '0') ; --pos, ++nchop );
            if( *pos == '.' ) ++nchop;
            s.chop(nchop);
        }
        return s;
    }

	friend class GraphicWidget;
};
